var express = require('express');
var app = express();
var bodyParser = require('body-parser');
const WebSocket = require('ws');
var http = require('http');
var os = require( 'os' );
var interfaces = os.networkInterfaces();

var sqlite = require('sqlite-sync');

const addresses = Object.keys(interfaces)
  .reduce((results, name) => results.concat(interfaces[name]), [])
  .filter((iface) => iface.family === 'IPv4' && !iface.internal)
  .map((iface) => iface.address); // pega o ip local

const portExpress = 8000;
const portWS = 5000;

var websocket;
var devices = [];
var auth = []
var log;

let get_class_time_for_a_week = (`SELECT  
		  p.Nome as authorized_name
		  , p.tag_id as authorized_tag
		  , a.hora_inicio as inicio
		  , a.hora_fim as fim
		  , a.nome_materia as materia
		  , a.week_day	as wd
		  FROM Servidor  as p
		  INNER JOIN  Aula  a ON (p.tag_id = a.tag_professor) 
		  WHERE a.n_sala = 1
		  UNION
		  SELECT
			p.Nome as authorized_name
			, p.tag_id as authorized_tag
			, a.hora_inicio as inicio
			, a.hora_fim as fim
			, a.nome_materia as materia
			, a.week_day	as wd
		  FROM Servidor  as p
		  INNER JOIN  Aula  a ON (p.Cargo = a.Cargo)`);

let get_esp = (
	`SELECT * FROM esp`
);


let get_users = (
	`SELECT * FROM Aluno`
);


function insert_in_to_database(id, tag, time, status){
	let result = ''	
	for (let row = 0; row < tag.length; row++){
		result += convert_to_hex(tag[row]).toString() + ' '
	}
	result = result.substring(0, result.length-1);	
	sqlite.run('INSERT INTO relatorio (esp_id, tag_id, timestamp, status) VALUES (?, ?, ?, ?)', [id, result, time, status],(err) => {console.log(err)});
};
	


function convert_to_hex(a){
	let value = (+a).toString(16).toUpperCase()
	if (value.length < 2 ){
		value = '0' + value 
	}
	return value;
}	

app.use(express.static('www'));


function delay(milliseconds) {
    var start = new Date().getTime();
    for (var i = 0; i < 1e7; i++) {
        if ((new Date().getTime() - start) > milliseconds){
            break;
        }
    }
}

function connect_db(){
	console.log('Abrindo Database')
	try {
	   sqlite.connect('database.db');
	   console.log('connectado com sucesso ao banco');
	}
	catch (e) {
	  	console.log('erro conectando ao banco ' + e)
	}
}
connect_db()

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

delay(5000);
		
wss.on('connection', function connection(ws) {
	devices.push(ws);
	
    ws.on('message', function incoming(MSG) {
        MSG = JSON.parse(MSG);
		if (MSG.type == 'CONNECT'){
			devices[devices.length-1].id = MSG.id;
			let id = MSG.id;
			let esp = sqlite.run(get_esp);
			let flag = 0
			for (let row = 0; row < esp.length; row++){
				if (esp[row].id == id){		
						flag = 1
						break;
					}
				else{ 
						flag = 0
					}
			}	
			obj = {id:id, auth:flag}
			auth.push(obj)
			if (flag){
				console.log('esp ' + id +' conectado')
				ws.send(JSON.stringify({type:'CONNECT', status:'OK'}));
			}
			else { 
				ws.send(JSON.stringify({type:'CONNECT', status:'ERRO'}));
				console.log('esp ' + id +' conexao rejeitada')
			}		
		}

		if (MSG.type == 'HOUR'){
			let id = MSG.id  
			let flag = 0
			for (let row = 0; row < auth.length; row++){
				if (id == auth[row].id && auth[row].auth == 1){
					flag = 1
					ws.send(JSON.stringify({type:'HOUR', status:(parseInt(new Date().getTime()/1000) - 10800)}));
				}
			}
			if(!flag){
				ws.send(JSON.stringify({type:'HOUR', status:'auth error'}));
			}
		}  

        if (MSG.type == 'UPDATE') // solicitando acesso a materia na semana
        {
			let id = MSG.id;
			let aulas = sqlite.run(get_class_time_for_a_week);
			
			if(MSG.status == 'REFRESH'){
				console.log('refresh')
				if (aulas.length == 0) {
					ws.send(JSON.stringify({type:'UPDATE', status:'null', new_line:0}));
				}
				else{
					for (let row = 0; row < auth.length; row++){
						if (id == auth[row].id && auth[row].auth == 1){
							auth[row].line = 0
							if (aulas.length == 1)
								ws.send(JSON.stringify({type:'UPDATE', status:aulas[0], new_line:0}));
							else ws.send(JSON.stringify({type:'UPDATE', status:aulas[0], new_line:1}));
							break;
						}
					}	
				}
			}

			if(MSG.status == 'OK'){
				for (let row = 0; row < auth.length; row++){
					if (id == auth[row].id && auth[row].auth == 1){
						auth[row].line++;
						if (aulas.length-1 > auth[row].line){
							ws.send(JSON.stringify({type:'UPDATE', status:aulas[auth[row].line], new_line:1}));
						}
						else if (aulas.length-1 == auth[row].line) {
							ws.send(JSON.stringify({type:'UPDATE', status:aulas[auth[row].line], new_line:0}));
						}						
					}
				}
			}
		}

		if(MSG.type == 'ACCESS'){	
			let time = MSG.time;
			let autorizados = sqlite.run(get_users)
			let id = MSG.id;
			let tag = MSG.tag;
			let status = MSG.status;
			tag = tag.split(' ')
			let result = ''
			console.log(MSG)
			for (let row = 0; row < tag.length; row++){
				result += convert_to_hex(tag[row]).toString() + ' '
			}
			result = result.substring(0, result.length-1);

			insert_in_to_database(id, tag, time, status)		
			let flag = 0 
			
			if (MSG.status == 'ONLINE'){
				for (let row = 0; row < autorizados.length; row++){
					if ((autorizados[row].tag_aluno).toString() == result.toString()){
						ws.send(JSON.stringify({type:'ACCESS', status:autorizados[row].nome}));
						flag = 1;
						break;
					 }
				}
				if (flag == 0)
					ws.send(JSON.stringify({type:'ACCESS', status:'null'}));
			}
			else if (MSG.status == 'OFFLINE'){
				ws.send(JSON.stringify({type:'ACCESS', status:'OK'}));
			}
		}
	
    });

    ws.on('close', function close() { // se alguem desconectar, retire da lista e finaliza o jogo

		console.log('Desconectar');
		
        var x = devices.indexOf(ws);	// remove usuario que saiu da pagina
            if (x != -1) {
                devices.splice(x, 1);
				log = ws.id + ' desconectou. Conectados: ' + devices.length;
				console.log(log);
            }
		
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


