static const char configWebPage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title></title>
  <meta charset=utf8>

  <style>
    body { color: #000; background-color: #fff; }
    body.black { color: #fff; background-color: #000; }
  </style>

  <script type="text/javascript">
  window.onload = function() {
    var $btn = document.querySelector('#station');
    var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);
    connection.onopen = function() { console.log('WebSocket connected'); };
    connection.onerror = function(error) { console.log('WebSocket Error ', error); };
    connection.onmessage = function(e) {
      if (e.data === 'hello') {
        document.body.classList.add('black');
      }
    };
    $btn.addEventListener('click', function(e) {
      connection.send('hello');
    });
  };
  </script>
</head>
<body>
  <div>
    <input type=button id=station value=station>  
  <div>
</body>

</html>
)=====";
