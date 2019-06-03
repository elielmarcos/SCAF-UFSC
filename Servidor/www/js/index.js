// Variáveis Globais
var meuID;
var websocket;
var TIMEOUT = 20000;
var N = 8;
var i, j;
var Troca;
var timeMSG;
var Flag=0;
var timeStart=0;
var msg_CONVITE;
var vez; //vez de quem jogar
var peca = new Array();
var il = new Array();


function inicia_jogo(){
	
	document.getElementById('lista_jogadores').style.display='none';
	document.getElementById('escolhecor-inicio').style.display='none';
	document.getElementById('fundo').style.display='none';
	document.getElementById('info_partida').style.display='block';
	
	var nomeAdv   = meuID.Adv;
	nomeAdv = nomeAdv.toString();
	nomeAdv = nomeAdv.split('#');
	
	document.getElementById('adversario_vez').innerHTML = 'ADVERSÁRIO: '+nomeAdv[0];	
	document.getElementById('cor_vez').innerHTML = 'SOU: '+ meuID.Cor;	
   
//muda a classe das pecas pretas(encima) para mostrar imgens das pecas
	document.getElementById("t11").innerHTML = "&#9820;";	// torre
	document.getElementById("t12").innerHTML = "&#9822;";	// cavalo
	document.getElementById("t13").innerHTML = "&#9821;";	// bispo
	document.getElementById("t14").innerHTML = "&#9819;";	// rei
	document.getElementById("t15").innerHTML = "&#9818;";	// rainha
	document.getElementById("t16").innerHTML = "&#9821;";	// bispo
	document.getElementById("t17").innerHTML = "&#9822;";	// cavalo
	document.getElementById("t18").innerHTML = "&#9820;";	// torre
	
	document.getElementById("t21").innerHTML = "&#9823;";	// peao
	document.getElementById("t22").innerHTML = "&#9823;";	// peao
	document.getElementById("t23").innerHTML = "&#9823;";	// peao
	document.getElementById("t24").innerHTML = "&#9823;";	// peao
	document.getElementById("t25").innerHTML = "&#9823;";	// peao
	document.getElementById("t26").innerHTML = "&#9823;";	// peao
	document.getElementById("t27").innerHTML = "&#9823;";	// peao
	document.getElementById("t28").innerHTML = "&#9823;";	// peao
	
//muda a classe das pecas brancas(embaixo) para mostrar imgens das pecas
	document.getElementById("t81").innerHTML = "&#9814;";	// torre
	document.getElementById("t82").innerHTML = "&#9816;";	// cavalo
	document.getElementById("t83").innerHTML = "&#9815;";	// bispo
	document.getElementById("t84").innerHTML = "&#9813;";	// rei
	document.getElementById("t85").innerHTML = "&#9812;";	//rainha
	document.getElementById("t86").innerHTML = "&#9815;";	// bispo
	document.getElementById("t87").innerHTML = "&#9816;";	// cavalo
	document.getElementById("t88").innerHTML = "&#9814;";	// torre
	
	document.getElementById("t71").innerHTML = "&#9817;";	// peao
	document.getElementById("t72").innerHTML = "&#9817;";	// peao
	document.getElementById("t73").innerHTML = "&#9817;";	// peao
	document.getElementById("t74").innerHTML = "&#9817;";	// peao
	document.getElementById("t75").innerHTML = "&#9817;";	// peao
	document.getElementById("t76").innerHTML = "&#9817;";	// peao
	document.getElementById("t77").innerHTML = "&#9817;";	// peao
	document.getElementById("t78").innerHTML = "&#9817;";	// peao
	
	//cria array que vai receber as posicoes do tabuleiro
	cria_array();
	function cria_array(){
		var x,y;

		peca = {}; 
		
		for(x=1;x<=8;x++){
			
			peca[x] = new Array();
			peca[x] = {}; 
			
			for(y=1;y<=8;y++){
				
			
				peca[x][y] = new Array();
				peca[x][y] = {};
				peca[x][y]['peca'] = false; 		//definido como falso para não começar com peça as que ficarem nulas
				peca[x][y]['cor'] = false;			//definido como falso para não começar com peça as que ficarem nulas
			 
			}
		}
		
		il['preto'] = new Array();
		il['branco'] = new Array();
	
	}	
		
//posiciona as pecas pretas no array
	peca[1][1]['peca']="torre";		peca[1][1]['cor']="preto";	 il['preto']['torre'] = "&#9820;";
	peca[1][2]['peca']="cavalo";	peca[1][2]['cor']="preto";	 il['preto']['cavalo'] = "&#9822;";
	peca[1][3]['peca']="bispo"; 	peca[1][3]['cor']="preto";	 il['preto']['bispo'] = "&#9821;";
	peca[1][4]['peca']="rainha";	peca[1][4]['cor']="preto";	 il['preto']['rainha'] = "&#9819;";
	peca[1][5]['peca']="rei";		peca[1][5]['cor']="preto";	 il['preto']['rei'] = "&#9818;";
	peca[1][6]['peca']="bispo";		peca[1][6]['cor']="preto";	
	peca[1][7]['peca']="cavalo";	peca[1][7]['cor']="preto";	
	peca[1][8]['peca']="torre";		peca[1][8]['cor']="preto";	

	peca[2][1]['peca']="peao";		peca[2][1]['cor']="preto";	 il['preto']['peao'] = "&#9823;";
	peca[2][2]['peca']="peao";		peca[2][2]['cor']="preto";	
	peca[2][3]['peca']="peao"; 		peca[2][3]['cor']="preto";	
	peca[2][4]['peca']="peao";		peca[2][4]['cor']="preto";	
	peca[2][5]['peca']="peao";		peca[2][5]['cor']="preto";	
	peca[2][6]['peca']="peao";		peca[2][6]['cor']="preto";	
	peca[2][7]['peca']="peao";		peca[2][7]['cor']="preto";	
	peca[2][8]['peca']="peao";		peca[2][8]['cor']="preto";	

//posiciona as pecas brancas no array	
	peca[8][1]['peca']="torre";		peca[8][1]['cor']="branco";	 il['branco']['torre'] = "&#9814;";
	peca[8][2]['peca']="cavalo";	peca[8][2]['cor']="branco";	 il['branco']['cavalo'] = "&#9816;";
	peca[8][3]['peca']="bispo"; 	peca[8][3]['cor']="branco";	 il['branco']['bispo'] = "&#9815;";
	peca[8][4]['peca']="rainha";	peca[8][4]['cor']="branco";	 il['branco']['rainha'] = "&#9813;";
	peca[8][5]['peca']="rei";		peca[8][5]['cor']="branco";	 il['branco']['rei'] = "&#9812;";
	peca[8][6]['peca']="bispo";		peca[8][6]['cor']="branco";	
	peca[8][7]['peca']="cavalo";	peca[8][7]['cor']="branco";	
	peca[8][8]['peca']="torre";		peca[8][8]['cor']="branco";	

	peca[7][1]['peca']="peao";		peca[7][1]['cor']="branco";	 il['branco']['peao'] = "&#9817;";
	peca[7][2]['peca']="peao";		peca[7][2]['cor']="branco";	
	peca[7][3]['peca']="peao"; 		peca[7][3]['cor']="branco";	
	peca[7][4]['peca']="peao";		peca[7][4]['cor']="branco";	
	peca[7][5]['peca']="peao";		peca[7][5]['cor']="branco";	
	peca[7][6]['peca']="peao";		peca[7][6]['cor']="branco";	
	peca[7][7]['peca']="peao";		peca[7][7]['cor']="branco";	
	peca[7][8]['peca']="peao";		peca[7][8]['cor']="branco";	


//array para movimentar as pecas
	movimenta = new Array();
	
	movimenta[0] = new Array();
	movimenta[0] = {};
	movimenta[0]['x'] =0;
	movimenta[0]['y'] =0;
	movimenta[0]['peca']="0";
	movimenta[0]['cor']="0";
	
	movimenta[1] = new Array();
	movimenta[1] = {};
	movimenta[1]['x'] =0;
	movimenta[1]['y'] =0; 
	movimenta[1]['peca'] ="0";
	movimenta[1]['cor']="0";
	
//array para os possiveis movimentos
	possiveis = new Array();

}

function possiveis_movimentos(){
		var x,y;
		var c =0; //contador pro array possiveis
		var i; //contador pros for
		x = movimenta[0]['x'];
		y = movimenta[0]['y'];

		document.getElementById('t'+x+y).style.backgroundColor = "#3C9"; //muda cor de fundo
		possiveis[c] = "t"+x+y; c++;

///////////////////////////////////////////////////////////////////////////////////PEAO////////////////////////////////
		if(peca[x][y]['peca']=='peao'){
			if(peca[x][y]['cor']=="branco"){

					if(!peca[x-1][y]['peca']){
						possivel(x-1,y);
					}if(y-1>0 && peca[x-1][y-1]['peca']){
						possivel(x-1,y-1);						
					}
					if(y+1<9 && peca[x-1][y+1]['peca']){
						possivel(x-1,y+1);					
					}					

					if(x==7){ // primeiro movimento pulo duplo
						if(!peca[x-2][y]['peca'] && !peca[x-1][y]['peca']){
							possivel(x-2,y);
						}
					}

			}
			
			if(peca[x][y]['cor']=="preto"){
					
					if(!peca[x+1][y]['peca']){
						possivel(x+1,y);
					}if(y-1>0 && peca[x+1][y-1]['peca']){
						possivel(x+1,y-1);						
					}
					if(y+1<9 && peca[x+1][y+1]['peca']){
						possivel(x+1,y+1);					
					}					
					
					if(x==2){ // primeiro movimento pulo duplo
					
						if(!peca[x+2][y]['peca'] && !peca[x+1][y]['peca']){
							possivel(x+2,y);
						}
			
					}
			}
		}
///////////////////////////////////////////////////////////////////////////////////////FIM PEAO//////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////CAVALO ////////////////////////////////

		if(peca[x][y]['peca']=='cavalo'){
			
			possivel(x-1,y-2);
			possivel(x+1,y+2);
			possivel(x+1,y-2);
			possivel(x-1,y+2);
			possivel(x-2,y-1);
			possivel(x+2,y+1);
			possivel(x+2,y-1);
			possivel(x-2,y+1);
			
		}
//////////////////////////////////////////////////////////////////////////////////////FIM CAVALO ////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////REI ///////////////////////////////////
		if(peca[x][y]['peca']=='rei'){
			possivel(x-1,y);
			possivel(x,y-1);
			possivel(x-1,y-1);
			possivel(x+1,y);
			possivel(x,y+1);
			possivel(x+1,y+1);
			possivel(x-1,y+1);
			possivel(x+1,y-1);
		}
//////////////////////////////////////////////////////////////////////////////////////FIM REI ////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////TORRE ///////////////////////////////////
		if(peca[x][y]['peca']=='torre'){
			
			for(i=1;possivel(x-i,y);i++);
			for(i=1;possivel(x+i,y);i++);
			for(i=1;possivel(x,y-i);i++);
			for(i=1;possivel(x,y+i);i++);
		}
//////////////////////////////////////////////////////////////////////////////////////FIM TORRE ////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////BISPO ///////////////////////////////////
		if(peca[x][y]['peca']=='bispo'){
			
			for(i=1;possivel(x-i,y-i);i++);
			for(i=1;possivel(x+i,y+i);i++);
			for(i=1;possivel(x-i,y+i);i++);
			for(i=1;possivel(x+i,y-i);i++);
		}
//////////////////////////////////////////////////////////////////////////////////////FIM BISPO ////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////RAINHA ///////////////////////////////////
		if(peca[x][y]['peca']=='rainha'){
			
			for(i=1;possivel(x-i,y-i);i++);
			for(i=1;possivel(x+i,y+i);i++);
			for(i=1;possivel(x-i,y+i);i++);
			for(i=1;possivel(x+i,y-i);i++);
			for(i=1;possivel(x-i,y);i++);
			for(i=1;possivel(x+i,y);i++);
			for(i=1;possivel(x,y-i);i++);
			for(i=1;possivel(x,y+i);i++);
			
		}
//////////////////////////////////////////////////////////////////////////////////////FIM RAINHA ////////////////////////////

		function possivel(px,py){
				if(px>0 && px <9 && py>0 && py <9 && peca[px][py]['cor']!= movimenta[0]['cor']){
					document.getElementById('t'+(px)+(py)).style.backgroundColor = "#3C9"; //muda cor de fundo
					possiveis[c] = "t"+(px)+(py); c++;
					
					if(!peca[px][py]['peca']){
						return true;
					}
				}else{
					return false;
				}
			}

	return c;
}

function volta_fundo(){
	var cf;
	for(cf=0;cf<possiveis.length;cf++){
		document.getElementById(possiveis[cf]).style.backgroundColor = ""; 
	}	
}

function verifica_possivel(x,y,c){ // faz a busca no vetor de possíveis verificando se tem a posição de destino neste vetor 
	var pode=0;
	var cp;
	var div = "t"+x+y;
	
	for(cp=1;cp<c;cp++){
		
		if(possiveis[cp]==div){
			pode ++;
		}
		if(pode>0){
			return 1;
		}
	}	
}

function seleciona(x,y){
	
	
	if (meuID.Cor == vez){	
		// ((primeira vez que clica(origem) == 0 || cliquei em outra peça minha) && é a minha vez de jogar)?
		if((movimenta[0]['x'] == 0 || peca[x][y]['cor'] == movimenta[0]['cor']) && peca[x][y]['cor']==vez){
			if(movimenta[0]['x']!=0){
				volta_fundo(); //volta a cor de fundo normal
			}
			if(peca[x][y]['peca']){ //se tiver uma peca nessa posição
				movimenta[0]['x'] = x;	//recebe x selecionado
				movimenta[0]['y'] = y;  //recebe y selecionado
				movimenta[0]['peca'] = peca[x][y]['peca']; //recebe a peca selecionada
				movimenta[0]['cor'] = peca[x][y]['cor'];	//recebe a cor selecionada
				
				cont_possiveis = possiveis_movimentos();	
			
			}
			
		}else if(verifica_possivel(x,y,cont_possiveis)){ //se for segundo clique e a cor da peca destino for diferente da selecionada
			
			if(peca[x][y]['cor'] != movimenta[0]['cor']){ // peça selecionada é diferente da peça de origem
				
				movimenta[1]['x'] = x;	//recebe o x do destino(segundo clique)
				movimenta[1]['y'] = y;  //recebe y do destino(segundo clique)
				
				if(peca[x][y]['peca']){  //se tiver alguma peca nessa posição
					movimenta[1]['peca'] = peca[x][y]['peca'];	//destino recebe a peca selecionada
					movimenta[1]['cor'] = peca[x][y]['cor'];	//destino recebe a cor selecionada
				}
		
				websocket.send(JSON.stringify({
							tipo: 'MOVIMENTO',
							valor: {peca,movimenta}
						}))
				
				vez = "";
				movimenta[0]['x'] =0;	//selecionada x recebe 0 (pra na proxima ver q é o primeiro movimento)
				movimenta[0]['y'] =0;	//selecionada y recebe 0 (pra na proxima ver q é o primeiro movimento)
				movimenta[0]['peca']="0";	//selecionada peca recebe 0 (pra na proxima ver q é o primeiro movimento)
				movimenta[0]['cor']="0";	//selecionada cor recebe 0 (pra na proxima ver q é o primeiro movimento)
				
				movimenta[1]['x'] =0;	//destino x recebe 0 (pra na proxima ver q é o primeiro movimento)
				movimenta[1]['y'] =0;	//destino y recebe 0 (pra na proxima ver q é o primeiro movimento)
				movimenta[1]['peca']="0";	//destino peca recebe 0 (pra na proxima ver q é o primeiro movimento)
				movimenta[1]['cor']="0";	//destino cor recebe 0 (pra na proxima ver q é o primeiro movimento)
				
			}
			volta_fundo(); //volta a cor de fundo normal
		}
	}
}

	function escolhe(pecae,core){
			peca[xe][ye]['peca']=pecae;
			document.getElementById("t"+xe+""+ye).innerHTML = il[core][pecae];
			document.getElementById('escolhe'+core).style.display='none';
			document.getElementById('fundo').style.display='none';
	}

	function escolhecor_incio(cor){
			
			document.getElementById('escolhecor-inicio').style.display='none';
			
			if (cor != 'nulo')
			{
				document.getElementById('fundo').style.display='none';
				document.getElementById('lista_jogadores').style.display='none';
				meuID.Cor = cor; //cor de quem joga
			}
			
			if (cor == 'preto' || cor == 'branco'){
				if(Date.now()-timeStart>TIMEOUT){
					troca_valor_div("TEMPO ESGOSTADO PARA RESPOSTA!");
					document.getElementById('fundo').style.display='block';
					document.getElementById('lista_jogadores').style.display='block';
				}else{
							meuID.Status = 'Jogando';
							meuID.Adv = msg_CONVITE.valor.remetente;
							msg_CONVITE.valor.Cor = meuID.Cor;
							websocket.send(JSON.stringify({
								tipo: 'ACEITA',
								valor: msg_CONVITE.valor
							}))
							clearTimeout(timeMSG);
							
						if(meuID.Cor=="branco"){vez="preto";}else{vez="branco";}
						inicia_jogo(); // inicia jogo
						document.getElementById('id_vez').innerHTML = 'VEZ: '+vez;
						
				}
				
			}else { // Caso recuse

				if(Date.now()-timeStart>TIMEOUT){
					troca_valor_div("TEMPO ESGOSTADO PARA RESPOSTA!");
					document.getElementById('fundo').style.display='block';
					document.getElementById('lista_jogadores').style.display='block';

				}else{
						msg_CONVITE.valor.nomeDestinatario = meuID.Nome;
						websocket.send(JSON.stringify({
							tipo: 'RECUSA',
							valor: msg_CONVITE.valor
						}))
						document.getElementById('fundo').style.display='block';
						document.getElementById('lista_jogadores').style.display='block';
				}
			}

	}
	
	function conectar(){
			document.getElementById('nome').style.display='none';
			
			var dataAtual = new Date();
            var tempo = dataAtual.getTime();
            var key = jogador.value +'#'+ tempo;
            var user = {
                ID: key,
                Nome: jogador.value,
                Status: 'Livre',
                Adv: '',
				Cor: ''
            };

            meuID = user;
            connect();
			console.log(meuID);
			document.getElementById('lista_jogadores').style.display='block';
			
	}
	
	function connect() {
		
            websocket = new WebSocket('ws://'+getParameterURL()+':8080');
            websocket.onopen = function() {
                onOpen();
            };
			
            websocket.onclose = function(e) {
                console.log('Socket is closed. Reconecta em 1s', e.reason);
                setTimeout(function() {
                    connectBAK();
                }, 2000);
            };
			
            websocket.onmessage = function(e) {
                console.log('Message:', e.data);
                onMessage(e);
            };
			
            websocket.onerror = function(err) {
                console.error('Erro encontrado no socket', err.message, 'Fechando socket');
            };
        };
		
		
		function connectBAK() {
            websocket = new WebSocket('ws://'+getParameterURL()+':7080');
            websocket.onopen = function() {
                onOpen();
            };
			
            websocket.onclose = function(e) {
                console.log('Socket is closed. Reconecta em 1s', e.reason);
                setTimeout(function() {
                    connect();
                }, 2000);
            };
			
            websocket.onmessage = function(e) {
                console.log('Message:', e.data);
                onMessage(e);
            };
			
            websocket.onerror = function(err) {
                console.error('Erro encontrado no socket', err.message, 'Fechando socket');
            };
        };

        function onOpen() {
            console.log('onOpen')
            var MSG = {
                tipo: 'CONECTAR',
                valor: meuID
            };
            websocket.send(JSON.stringify(MSG));
        };
		
		
		function onMessage(evt) {
            var msg = evt.data
            msg = JSON.parse(msg);
            switch (msg.tipo) {
                case 'CONECTADO': // Resposta do servidor quando conectou
                    console.log('Conectou');
                    break;
				case 'LISTA': // Recebe a lista do jogadores atualizada
                    RefreshCombo(msg); // Atualiza combobox
                    break;
				case 'CONVITE': // Recebe o convite do Remetente para jogar 
				    timeStart = Date.now();
					msg_CONVITE = msg;
					mostra_convite('Convite de '+ msg.valor.nomeRemetente+', aceita?');
					break;
				case 'ACEITOU': // Recebe confirmação de convite aceito para jogar
                    meuID.Adv = msg.valor.destinatario;
					meuID.Cor = msg.valor.Cor;
                    meuID.Status = 'Jogando';
					clearTimeout(timeMSG);
					vez = msg.valor.Cor;
					document.getElementById('id_vez').innerHTML = 'VEZ: '+vez;
                    inicia_jogo(); // inicia jogo
                    break;
                case 'RECUSOU': // Recebe a recusa de convite para jogar
					clearTimeout(timeMSG);
					document.getElementById('lista_jogadores').style.display='block';
                    troca_valor_div(msg.valor.nomeDestinatario + ' nao aceitou seu convite, tente outro jogador.');
                    break;
				case 'MOVIMENTO':
					console.log('MENSAGEM RECEBIDA PELO SERVIDOR: \t');
					console.log(msg.valor.peca);
					peca = msg.valor.peca;
					posicionar_pecas();
					vez = msg.valor.vez;
					document.getElementById('id_vez').innerHTML = 'VEZ: '+vez;
					break;
				case 'FINALIZAR': // Recebe mensagem que adversário finalizou jogo
                    troca_valor_div(msg.valor.nomeRemetente + ' decidiu finalizar o jogo.');
                    finalizarJogo();
                    break;
				case 'GANHOU':
					if(msg.valor == meuID.ID){
						troca_valor_div('PARABÉNS, VOCÊ GANHOU! :)');
					}else {
						troca_valor_div('VOCÊ PERDEU. :(');
					}
					finalizarJogo();
					break;
            }
            console.log('Recebeu msg');
        };
		
		function finalizarJogo() { // finaliza jogo, limpa tabuleiro e botões

            meuID.Status = 'Livre';
            meuID.Adv = '';
			meuID.Cor = '';
			
			for (var i = 1; i <= 8; i++){
				for(var j = 1; j <= 8; j++){
					document.getElementById("t"+i+j).innerHTML = ""; 
				}
			}
			
			document.getElementById('lista_jogadores').style.display='block';
			document.getElementById('fundo').style.display='block';
			document.getElementById('info_partida').style.display='none';
			
		}
		
        function RefreshCombo(Dados) // Atualiza combobox com a lista atualizada recebida
        {
            var combo = document.getElementById('adversario');
            //LIMPA COMBO-BOX
            while (combo.length) {
                combo.remove(0);
            }
            //ADICIONA USUARIOS AO COMBO-BOX
            var k = 0;
            for (let j = 0; j < Dados.valor.length; j++) {
                if (Dados.valor[j].ID != meuID.ID) {
                    combo.options[k] = new Option(Dados.valor[j].Nome + ' - ' + Dados.valor[j].Status, Dados.valor[j].ID);
                    if (Dados.valor[j].Status == 'Livre') {
                        combo[k].style = "background-color:#0de26d";
                    } else {
                        combo[k].style.color = "red";
                    }
                    k++;
                }
				else
				{
				meuID.Status = Dados.valor[j].Status;
				meuID.Adv = Dados.valor[j].Adv;
				meuID.Cor = Dados.valor[j].Cor;
				}
            }
        }
		
        function convidar() { // Fazer um convite de jogo

            if (adversario.options[adversario.selectedIndex].text.match(/Livre/)) {
                var Convite = {
                    destinatario: adversario.options[adversario.selectedIndex].value,
                    remetente: meuID.ID,
					nomeRemetente: meuID.Nome
                };
                var CNV = {
                    tipo: 'CONVIDAR',
                    valor: Convite
                };				
                websocket.send(JSON.stringify(CNV));
				document.getElementById('lista_jogadores').style.display='none';
					timeMSG = setTimeout(function(){
					document.getElementById('lista_jogadores').style.display='block';
					troca_valor_div('TEMPO ESGOTADO! Tente novamente...');
				},TIMEOUT);
                troca_valor_div('Convite enviado, aguarde.');
            } else {
                troca_valor_div('Jogador ocupado, tente outro adversario');
            }
        };

		function OK(){
			document.getElementById('id_generico_pai').style.display='none';
		};
		
		
		function troca_valor_div(texto){
			document.getElementById('id_generico_filho').innerHTML = texto;	
			document.getElementById('id_generico_pai').style.display='block';
		}
		
		function mostra_convite(adversario){			
			document.getElementById('escolhecor-inicio-adv_text').innerHTML = adversario;
			document.getElementById('escolhecor-inicio').style.display='block';	
		}
		
		function posicionar_pecas(){
			
			for(var x = 1; x <=8; x++){
				for(var y = 1; y <= 8; y++){
					if(peca[x][y]['peca'] == false){
						document.getElementById("t"+x+y).innerHTML = ""; //selecionada fica sem imagem
					}else{
						document.getElementById("t"+x+y).innerHTML = il[peca[x][y]['cor']][peca[x][y]['peca']]; //destino recebe a imagem da peça selecinada
					}
				}
			}
		}
		
		function Botao_FinalizarJogo(){
			
				let FIM = {
					tipo: 'FINALIZAR',
					valor: {
						destinatario: meuID.Adv,
						remetente: meuID.ID,
						nomeRemetente: meuID.Nome
					}
				};
				websocket.send(JSON.stringify(FIM));
				finalizarJogo();
		}
		
		
        //Acessamos a funcao passando o parametros que queremos o valor do seu retorno .Ex: o parametro [stars]

		function getParameterURL() {
			var url   = window.location;
			url = url.toString();
			var items = url.split('://');
			items = items[1].split(':');
			//alert(items[0]);
			return items[0];
		}
  
		
		
		