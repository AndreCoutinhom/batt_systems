const char INDEX_HTML[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="pt-br">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <title>Batt – Controle do detector de obstáculos</title>
  <link rel="stylesheet" href="/styles.css">
  <style>
    /* Texto lido por leitores de tela, invisível na tela */
    .sr-only{
      position:absolute; width:1px; height:1px; padding:0; margin:-1px;
      overflow:hidden; clip:rect(0,0,0,0); white-space:nowrap; border:0;
    }
    #boas-vindas:focus{ outline:none; }
  </style>
</head>
<body class="bg">
  <main class="wrap">
    <section class="card">
      <header class="card__head">
        <!-- 1) Boas-vindas: recebe o foco ao abrir a página (ver batt.js) -->
        <h1 class="title" id="boas-vindas" tabindex="-1">
          <span aria-hidden="true">Batt</span>
          <span class="sr-only">
            Olá! Boas vindas ao Batt, a interface de controle do seu detector de obstáculos.
            Para continuar, deslize o dedo para a direita.
          </span>
        </h1>
        <!-- Informações visuais: irrelevantes para quem usa leitor de tela -->
        <p id="connection-status" class="muted" aria-hidden="true">Conectando ao ESP32...</p>
      </header>

      <div class="reading" aria-hidden="true">
        <p class="muted">Distância Atual</p>
        <p class="distance"><span id="distance-value">--</span> <span class="unit">cm</span></p>
        <div id="level-indicator" class="badge badge--gray">
          <span id="level-text">Aguardando</span>
        </div>
        <p class="tiny">Acima de <span id="fora-valor">100</span> cm ⇒ Fora de Alcance</p>
        <p class="micro">Regra inversa: <b>Alto</b> quando ≤ Alto; <b>Médio</b> quando ≤ Médio; <b>Leve</b> quando ≤ Leve; acima do Leve ⇒ Fora.</p>
      </div>

      <hr class="sep" aria-hidden="true" />

      <!-- 2) Próximo item após as boas-vindas: direto ao ajuste dos limites -->
      <h2 class="subtitle" id="ajuste-titulo">Ajustar Limites (cm)</h2>

      <!-- 3) Explicação didática -->
      <div class="sr-only" id="explicacao">
        <p>
          Aqui você escolhe a distância de cada um dos três níveis de vibração do dispositivo:
          suave, moderado e forte. Eles representam a intensidade com que o dispositivo vai vibrar
          quando houver um obstáculo naquela distância. O nível suave vibra levemente, quando o
          obstáculo ainda está longe. O nível moderado vibra um pouco mais forte, quando ele se aproxima.
          O nível forte vibra com a maior intensidade, quando o obstáculo está bem perto.
        </p>
        <p>
          Para ajustar, deslize o dedo para a direita até chegar ao deslizador de um nível.
          Depois, deslize o dedo para cima para aumentar a distância, ou para baixo para diminuí-la.

          Quando você parar de ajustar por alguns segundos, eu vou dizer como estão configuradas as três distâncias.
        </p>
      </div>


      <div class="control">
        <label for="leve-slider" class="row" aria-hidden="true">
          <span>Vibração Suave (aciona até)</span>
          <span id="leve-value" class="value">100 cm</span>
        </label>
        <input type="range" id="leve-slider" min="1" max="200" step="1" value="100"
               aria-label="Vibração Suave">
      </div>

      <div class="control">
        <label for="medio-slider" class="row" aria-hidden="true">
          <span>Vibração Moderada (aciona até)</span>
          <span id="medio-value" class="value">50 cm</span>
        </label>
        <input type="range" id="medio-slider" min="1" max="200" step="1" value="50"
               aria-label="Vibração Moderada">
      </div>

      <div class="control">
        <label for="alto-slider" class="row" aria-hidden="true">
          <span>Vibração Forte (aciona até)</span>
          <span id="alto-value" class="value">10 cm</span>
        </label>
        <input type="range" id="alto-slider" min="1" max="200" step="1" value="10"
               aria-label="Vibração Forte">
      </div>
    </section>
  </main>

  <div id="resumo-falado" class="sr-only" role="status" aria-live="polite" aria-atomic="true"></div>

  <script src="/app.js"></script>
</body>
</html>

)rawliteral";
