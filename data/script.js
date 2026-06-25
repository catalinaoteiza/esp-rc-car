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

let JoyLeft;
let JoyRight;

// =====================================================
// SERVO SLIDERS
// =====================================================

const servoLeftTrack =
    document.getElementById("servoLeftTrack");

const servoLeftKnob =
    document.getElementById("servoLeftKnob");

const servoRightTrack =
    document.getElementById("servoRightTrack");

const servoRightKnob =
    document.getElementById("servoRightKnob");

const servoLeftValue =
    document.getElementById("servoLeftValue");

const servoRightValue =
    document.getElementById("servoRightValue");

let servoLeftPos = 90;
let servoLeftTouchId = null;

let servoRightPos = 90;
let servoRightTouchId = null;

function updateServoLeft(clientX)
{
    const rect =
        servoLeftTrack.getBoundingClientRect();

    let x = clientX - rect.left;

    x = Math.max(0, Math.min(rect.width, x));

    servoLeftKnob.style.left =
        (x - 12) + "px";

    servoLeftPos = Math.round(
        (x / rect.width) * 180
    );

    servoLeftValue.textContent =
        servoLeftPos + "°";
}

function updateServoRight(clientX)
{
    const rect =
        servoRightTrack.getBoundingClientRect();

    let x = clientX - rect.left;

    x = Math.max(0, Math.min(rect.width, x));

    servoRightKnob.style.left =
        (x - 12) + "px";

    servoRightPos = Math.round(
        (x / rect.width) * 180
    );

    servoRightValue.textContent =
        servoRightPos + "°";
}
  

// =====================================================
// CONTROL PACKET GENERATION
// =====================================================
    let invertForward = false;
    let invertStrafe  = false;
    let invertRotate  = false;


    function deadzone(value)
    {
        return Math.abs(value) < 10 ? 0 : value;
    }

    function getControlData()
    {
            if (!JoyLeft || !JoyRight)
            {
                return "0,0,0,90,90";
            }

        // Stick izquierdo
        let leftX  = Number(JoyLeft.GetX());
        let leftY  = Number(JoyLeft.GetY());
        
        // Stick derecho
        let rightX = Number(JoyRight.GetX());
        let rightY = Number(JoyRight.GetY());

        // Asistencia direccional

        if (Math.abs(leftY) > Math.abs(leftX) * 2)
        {
            leftX = 0;
        }

        if (Math.abs(leftX) > Math.abs(leftY) * 2)
        {
            leftY = 0;
        }

        if (Math.abs(rightY) > Math.abs(rightX) * 2)
        {
            rightX = 0;
        }

        if (Math.abs(rightX) > Math.abs(rightY) * 2)
        {
            rightY = 0;
        }

        let forward = deadzone(leftY);
        let strafe  = deadzone(leftX);

        let rotate  = deadzone(-rightX);
        let aux     = deadzone(rightY);        

        if(invertForward) forward = -forward;
        if(invertStrafe)  strafe  = -strafe;
        if(invertRotate)  rotate  = -rotate;        

        if(!enabled)
        {
            forward = 0;
            strafe = 0;
            rotate = 0;
            aux = 0; 

            return "0,0,0,90,90";
        }

        if (isNaN(servoLeftPos))  servoLeftPos = 90;
        if (isNaN(servoRightPos)) servoRightPos = 90;

        return (
            forward +
            "," +
            strafe +
            "," +
            rotate +
            "," +
            servoLeftPos +
            "," +
            servoRightPos
        );
    }

    document
        .getElementById("configBtn")
        .onclick = () =>
        {
            document
                .getElementById("configPanel")
                .classList
                .toggle("hidden");
        };

// =====================================================
// PERIODIC TRANSMISSION
// =====================================================

    let lastPacket = "";

    setInterval(() =>
    {
        let packet = getControlData();

        if(packet !== lastPacket)
        {
            console.log(packet);

            send(packet);

            lastPacket = packet;
        }

    }, 50);

document.addEventListener(
    "touchmove",
    function(e)
    {
        if (
            e.target.closest("#joyLeft") ||
            e.target.closest("#joyRight")
        )
        {
            e.preventDefault();
        }
    },
    { passive: false }
);

let enabled = false;

// =====================================================
// BUTTONS
// =====================================================

function updateEnableButton()
{
    document.getElementById("enableBtn").textContent =
        enabled
        ? "DISABLE"
        : "ENABLE";
}

document
.getElementById("enableBtn")
.onclick = () =>
{
    enabled = !enabled;
    updateEnableButton();
};


document
.getElementById("stopBtn")
.onclick = () =>
{
    enabled = false;

    servoLeftPos = 90;
    servoRightPos = 90;
    console.log("Stop btn");
    send("0,0,0,90,90");
};

// =====================================================
// WEBSOCKET EVENTS
// =====================================================


ws.onopen = () =>
{
    updateConnectionStatus(true);
    console.log("WebSocket conectado");
};

ws.onclose = () =>
{
    updateConnectionStatus(false);
    console.log("WebSocket cerrado");
};

ws.onerror = (e) =>
{
    updateConnectionStatus(false);
    console.log("WebSocket error", e);
};


function updateConnectionStatus(connected)
{
    const status = document.getElementById("status");
    const label = status.querySelector(".label");

    status.classList.remove("connected", "disconnected");

    if (connected) {
        status.classList.add("connected");
        label.textContent = "Connected";
    } else {
        status.classList.add("disconnected");
        label.textContent = "Disconnected";
    }
}


// =====================================================
// INITIALIZATION
// =====================================================

window.addEventListener("load", () =>
{
    document.getElementById("invertForward").onchange = (e) =>
    {
        invertForward = e.target.checked;
        console.log("invert F:", invertForward);
    };

    document.getElementById("invertStrafe").onchange = (e) =>
    {
        invertStrafe = e.target.checked;
        console.log("invert S:", invertStrafe);
    };

    document.getElementById("invertRotate").onchange = (e) =>
    {
        invertRotate = e.target.checked;
        console.log("invert R:", invertRotate);
    };

    updateServoLeft(
        servoLeftTrack.getBoundingClientRect().left +
        servoLeftTrack.getBoundingClientRect().width / 2
    );

    updateServoRight(
        servoRightTrack.getBoundingClientRect().left +
        servoRightTrack.getBoundingClientRect().width / 2
    );



    servoLeftTrack.addEventListener(
        "touchstart",
        (e) =>
        {
            const touch = e.changedTouches[0];

            servoLeftTouchId = touch.identifier;

            updateServoLeft(touch.clientX);
        },
        { passive: false }
    );

    servoLeftTrack.addEventListener(
        "touchmove",
        (e) =>
        {
            for(const touch of e.changedTouches)
            {
                if(touch.identifier === servoLeftTouchId)
                {
                    updateServoLeft(touch.clientX);
                    break;
                }
            }
        },
        { passive: false }
    );

    servoLeftTrack.addEventListener(
        "touchend",
        (e) =>
        {
            for(const touch of e.changedTouches)
            {
                if(touch.identifier === servoLeftTouchId)
                {
                    servoLeftTouchId = null;
                    break;
                }
            }
        }
    );

    servoRightTrack.addEventListener(
        "touchstart",
        (e) =>
        {
            const touch = e.changedTouches[0];

            servoRightTouchId = touch.identifier;

            updateServoRight(touch.clientX);
        },
        { passive: false }
    );

    servoRightTrack.addEventListener(
        "touchmove",
        (e) =>
        {
            for(const touch of e.changedTouches)
            {
                if(touch.identifier === servoRightTouchId)
                {
                    updateServoRight(touch.clientX);
                    break;
                }
            }
        },
        { passive: false }
    );

    servoRightTrack.addEventListener(
        "touchend",
        (e) =>
        {
            for(const touch of e.changedTouches)
            {
                if(touch.identifier === servoRightTouchId)
                {
                    servoRightTouchId = null;
                    break;
                }
            }
        }
    );

    // =========================
    // JOYSTICKS
    // =========================

    createJoysticks();
});


function createJoysticks()
{
    const left = document.getElementById("joyLeft");
    const right = document.getElementById("joyRight");

    left.innerHTML = "";
    right.innerHTML = "";

    JoyLeft = new JoyStick("joyLeft", {});

    JoyRight = new JoyStick(
        "joyRight",
        {
            title: "joystick2"
        }
    );
}


let resizeTimer;

window.addEventListener("resize", () =>
{
    clearTimeout(resizeTimer);

    resizeTimer = setTimeout(() =>
    {
        JoyLeft.Resize();
        JoyRight.Resize();
    }, 200);
});


statusDiv.classList.remove("connected", "disconnected");

if (isConnected) {
    statusDiv.classList.add("connected");
    statusDiv.querySelector(".label").textContent = "Connected";
} else {
    statusDiv.classList.add("disconnected");
    statusDiv.querySelector(".label").textContent = "Disconnected";
}