var loc = window.location;
var WS_URL = (loc.protocol === "https:" ? "wss://" : "ws://") + window.location.hostname + ":82";
var ws = new WebSocket(WS_URL);

 
var lastText, lastSend, sendTimeout;
// limit sending to one message every 30 ms
// https://github.com/neonious/lowjs_esp32_examples/blob/master/neonious_one/cellphone_controlled_rc_car/www/index.html
function send(txt)
{
    var now = Date.now();

    if (lastSend === undefined || now - lastSend >= 30)
    {
        if (ws && ws.readyState === WebSocket.OPEN)
        {
            ws.send(txt);
            lastSend = Date.now();
            return;
        }
    }

    lastText = txt;

    if (!sendTimeout)
    {
        var ms = lastSend !== undefined ? 30 - (now - lastSend) : 30;

        if (ms < 0)
            ms = 0;

        sendTimeout = setTimeout(() =>
        {
            sendTimeout = null;
            send(lastText);
        }, ms);
    }
}


ws.onopen = () =>
{
    console.log("WebSocket conectado");
};

ws.onclose = () =>
{
    console.log("WebSocket cerrado");
};

ws.onerror = (e) =>
{
    console.log("WebSocket error", e);
};

 


