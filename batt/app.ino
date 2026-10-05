const char APP_JS[] PROGMEM = R"rawliteral(

const WEBSOCKET_URL = `ws://${window.location.hostname}:81/`; // porta 81

const connectionStatusEl = document.getElementById('connection-status');
const distanceValueEl = document.getElementById('distance-value');
const levelIndicatorEl = document.getElementById('level-indicator');
const levelTextEl = document.getElementById('level-text');

const leveSlider = document.getElementById('leve-slider');
const medioSlider = document.getElementById('medio-slider');
const altoSlider  = document.getElementById('alto-slider');

const leveValueEl = document.getElementById('leve-value');
const medioValueEl = document.getElementById('medio-value');
const altoValueEl  = document.getElementById('alto-value');
const foraValorEl  = document.getElementById('fora-valor');

let ws;

function badge(text, color){
  levelTextEl.textContent = text;
  levelIndicatorEl.className = 'badge ' +
    (color==='green' ? 'badge--green' :
     color==='orange'? 'badge--orange' :
     color==='red'   ? 'badge--red'    : 'badge--gray');
}

function L(){return parseInt(leveSlider.value)}
function M(){return parseInt(medioSlider.value)}
function A(){return parseInt(altoSlider.value)}

// Texto falado: "1 centímetro" / "N centímetros"
function cmTxt(v){ return `${v} ${v===1 ? 'centímetro' : 'centímetros'}`; }

// Durante o ajuste, o leitor fala só o valor curto do nível (ex.: "80 centímetros")
function updateSpeech(){
  leveSlider.setAttribute('aria-valuetext',  cmTxt(L()));
  medioSlider.setAttribute('aria-valuetext', cmTxt(M()));
  altoSlider.setAttribute('aria-valuetext',  cmTxt(A()));
}

const SUMMARY_DELAY_MS = 4500;
const speechEl = document.getElementById('resumo-falado');
let summaryTimer = null;

function scheduleSummary(){
  clearTimeout(summaryTimer);
  if (speechEl) speechEl.textContent = '';
  summaryTimer = setTimeout(()=>{
    if (speechEl) speechEl.textContent =
      `Configuração atual: vibração suave a partir de, ${cmTxt(L())}. vibração moderada a partir de, ${cmTxt(M())}. vibração forte a partir de, ${cmTxt(A())}.`;
  }, SUMMARY_DELAY_MS);
}

function updateDisplays(){
  const l=L(), m=M(), a=A();
  leveValueEl.textContent = `${l} cm`;
  medioValueEl.textContent = `${m} cm`;
  altoValueEl.textContent  = `${a} cm`;
  foraValorEl.textContent  = `${l}`;
  updateSpeech();
}

function clampInverse(){
  let l=L(), m=M(), a=A();
  if (m>l){ m=l; medioSlider.value=m; }
  if (a>m){ a=m; altoSlider.value=a; }
  updateDisplays();
}

function sendThresholds(){
  if (ws && ws.readyState===WebSocket.OPEN){
    ws.send(JSON.stringify({ limiteLeve:L(), limiteMedio:M(), limiteAlto:A(), units:'cm' }));
  }
}

function connectWS(){
  ws = new WebSocket(WEBSOCKET_URL);

  ws.onopen = () => { connectionStatusEl.textContent='Conectado ao ESP32'; sendThresholds(); };
  ws.onclose = () => { connectionStatusEl.textContent='Desconectado. Tentando reconectar...'; badge('Aguardando','gray'); setTimeout(connectWS,1000); };
  ws.onerror = () => { try{ws.close();}catch(e){} };

  ws.onmessage = (e) => {
    try{
      const d = JSON.parse(e.data);
      if (typeof d.distance==='number'){ distanceValueEl.textContent = `${Math.round(d.distance)}`; }
      if (d.level){
        const L = (d.level+'').toUpperCase();
        if (L.includes('ALTO')) badge('Forte','red');
        else if (L.includes('MED')) badge('Moderado','orange');
        else if (L.includes('LEV')) badge('Suave','green');
        else badge('Fora de Alcance','gray');
      }
    }catch(err){ console.error(err); }
  };
}

leveSlider.addEventListener('input',()=>{ clampInverse(); scheduleSummary(); });
medioSlider.addEventListener('input',()=>{ clampInverse(); scheduleSummary(); });
altoSlider .addEventListener('input',()=>{ clampInverse(); scheduleSummary(); });

leveSlider.addEventListener('change',sendThresholds);
medioSlider.addEventListener('change',sendThresholds);
altoSlider .addEventListener('change',sendThresholds);

window.addEventListener('load',()=>{
  clampInverse();
  connectWS();
  // Move o foco para a mensagem de boas-vindas para o leitor de tela falá-la
  const boasVindas = document.getElementById('boas-vindas');
  if (boasVindas) boasVindas.focus({ preventScroll: true });
});

)rawliteral";
