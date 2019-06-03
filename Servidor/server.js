var express = require('express');
var app = express();
var bodyParser = require('body-parser');
const WebSocket = require('ws');
var http = require('http');
var os = require( 'os' );
var interfaces = os.networkInterfaces();

const addresses = Object.keys(interfaces)
  .reduce((results, name) => results.concat(interfaces[name]), [])
  .filter((iface) => iface.family === 'IPv4' && !iface.internal)
  .map((iface) => iface.address); // pega o ip local

const portExpress = 8000;
const portWS = 5000;

var websocket;
var devices = [];
var log;

var flag = 0;

app.use(express.static('www'));

// Servidor disponibiliza uma porta para a conexão websocket
const wss = new WebSocket.Server({port : portWS}, function() {
    console.log('WEBSOCKET RODANDO NA PORTA http://%s:%s', addresses,portWS);
});

// porta 8080: porta de conexão websocket
// porta 8000: porta da página requisitada

var server = app.listen(portExpress , function () {
    var port = server.address().port;
    console.log('EXPRESS RODANDO NA PORTA http://%s:%s', addresses, port);
});


		
wss.on('connection', function connection(ws) {

	//ws.id = '';
	//ws.room = '';
	//console.log(ws);
	devices.push(ws);

    ws.on('message', function incoming(MSG) {
		
        MSG = JSON.parse(MSG);
		
        if (MSG.type == 'CONNECT') // Novo usuario pedindo para conectar
        {
			console.log('Conectar: ');

				devices[devices.length - 1].id = MSG.id;
				//devices[devices.length - 1].room = MSG.room;
            

			log = '    ' + devices[devices.length - 1].id + ' conectou.';
            console.log(log);

			
			log = '    ==> Conectado(s): ' + devices.length + '\n';
            console.log(log);
			
			 // Responde que foi conectado
            ws.send(JSON.stringify({type: 'CONNECT', status: 'OK'}));
			
			//ws.ping();
			

        }
		
		
		if (MSG.type == 'ACCESS') // Comando do usuario
        {

			console.log('Access: ');

			if (MSG.status == 'ONLINE')
			ws.send(JSON.stringify({type: 'ACCESS', status: 'Eliel Marcos Romancini'}));
			
			if (MSG.status == 'OFFLINE')
				ws.send(JSON.stringify({type: 'ACCESS', status: 'OK'}));
			
			log = '    ' + MSG + '\n';
            console.log(MSG);			

		}
		
		if (MSG.type == 'HOUR') // Comando do usuario
        {

			console.log('Hora: ');

			ws.send(JSON.stringify({type: 'HOUR', status:(parseInt(new Date().getTime()/1000) - 10800)}));

			
			
			log = '    ' + MSG.id + ' Hora: ' + MSG.status+'\n';
            console.log(log);			

		}
		
		if (MSG.type == 'UPDATE') // Comando do usuario
        {

			console.log('Update: ');

			if(MSG.status == 'REFRESH'){
				ws.send(JSON.stringify({type:'UPDATE', status:{authorized_name:'ELIEL',authorized_tag:'BA 49 29 0C',inicio:'0',fim:'0',materia:'null',wd:-1}, new_line:1}));
			}
			
			if(MSG.status == 'OK' && !flag){
				ws.send(JSON.stringify({type:'UPDATE', status:{authorized_name:'ELIEL MARCOS',authorized_tag:'C6 FF A1 BB',inicio:'17:01:00',fim:'20:20:00',materia:'(DEC-7000) Sistemas Ubiquos',wd:0}, new_line:0}));
				flag = 1;
			} else {flag = 0;}
			
			log = '    ' + MSG.id + ' Updade: ' + MSG.status+'\n';
            console.log(log);			

		}

    });

    ws.on('close', function close() { // se alguem desconectar, retire da lista e finaliza o jogo

		console.log('Desconectar: ');
		
        var x = devices.indexOf(ws);	// remove usuario que saiu da pagina
            if (x != -1) {
                devices.splice(x, 1);
				log = '    ' + ws.id + ' desconectou.';
				console.log(log);
            }
		
		 log = '    ==> Conectado(s): ' + devices.length + '\n';
		 console.log(log);
		
    });
	
});



function fazBroadcast(msg) {
    for (var x = 0; x < devices.length; x++) {
        try {
            devices[x].send(JSON.stringify(msg));
        } catch (e) {

        }
    }
}


function BroadcastPing() {
    for (var x = 0; x < devices.length; x++) {
        try {
            devices[x].ping();
        } catch (e) {

        }
    }
}


setInterval(BroadcastPing, 5000);


/*

Ap dos meninos
capadocia
Connnection Opened
Coneccao com servidor ACEITA!
Got Message: {"type":"CONNECT","status":"OK"}
Got a Ping!
Conectado.
WS Conectado
Hora: 1556743249
Got Message: {"type":"HOUR","status":1556743249}
Conectado.
WS Conectado
Conectado.
WS Conectado
Conectado.
WS Conectado
Got Message: {"type":"UPDATE","status":{"authorized_name":"ELIEL","authorized_tag":"BA 49 29 0C","inicio":"16:00:00","fim":"16:20:00","materia":"Servidor","wd":1},"new_line":1}
Got Message: {"type":"UPDATE","status":{"authorized_name":"ERRIEL","authorized_tag":"B6 16 25 07","inicio":"0","fim":0,"materia":"Servidor"},"new_line":1}
Got Message: {"type":"UPDATE","status":{"authorized_name":"GABS","authorized_tag":"C6 FF A1 B1","inicio":"0","fim":0,"materia":"Servidor"},"new_line":1}
Got Message: {"type":"UPDATE","status":{"authorized_name":"VINI","authorized_tag":"F7 F8 CC 7C","inicio":"0","fim":0,"materia":"Servidor"},"new_line":0}
Conectado.
WS Conectado
Conectado.
WS Conectado
PICC type: MIFARE 1KB
A new card has been detected.
 FA 2B 28 0CConectado.
WS Conectado
Got Message: {"type":"ACCESS","status":"CAPA"}
Conectado.
WS Conectado
Conectado.
WS Conectado
PICC type: MIFARE 1KB
A new card has been detected.
 F7 F8 CC 7CConectado.
WS Conectado
Got a Ping!
Got Message: {"type":"ACCESS","status":"VINI"}
Conectado.
WS Conectado
Conectado.
WS Conectado
PICC type: MIFARE 1KB
A new card has been detected.
 BA 49 29 0CConectado.
WS Conectado
Got Message: {"type":"ACCESS","status":"ELIEL\n"}
Conectado.
WS Conectado
Conectado.
WS Conectado
PICC type: MIFARE 1KB
A new card has been detected.
 B6 16 25 07Conectado.
WS Conectado
Got Message: {"type":"ACCESS","status":"ERRIEL"}
Conectado.
WS Conectado
Conectado.
WS Conectado
PICC type: MIFARE 1KB
A new card has been detected.
 C6 FF A1 BBConectado.
WS Conectado
Got Message: {"type":"ACCESS","status":"null"}
Conectado.
WS Conectado
Conectado.
WS Conectado
PICC type: MIFARE 1KB
A new card has been detected.
 C6 FF A1 BBConectado.
WS Conectado
Got Message: {"type":"ACCESS","status":"null"}
Conectado.
WS Conectado

*/




