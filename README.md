AUTOMAÇÃO RESIDENCIAL COM ESP32 E NODE-RED

Atualmente as pessoas estão buscando cada vez mais comodidade e praticidade, fazendo com que a automação residencial se torne uma opção capaz de trazer mordomia e facilidade para essas pessoas. Inicialmente a automação residencial começou como um produto de difícil acesso e alto custo, porém, essa realidade está mudando. Com o surgimento de novas tecnologias, a aplicação da automação tem se tornado cada vez mais acessível, como consequência, barateando e aumentando sua viabilidade.

Esse trabalho propõe elaborar um sistema de automação residencial eficiente, que pode ser acessado de qualquer lugar que você esteja.

Para realizar esse projeto, é preciso estabelecer objetivos específicos:

-Criar uma instância EC2 na Amazon Web Services (AWS);
-Instalar Apache, MySQL e PHP na instância;
-Criar um RDS na AWS e configurar ele através do MySQL;
-Instalar o Node-RED na instância EC2;
-Utilizar o protocolo MQTT no Node-RED e no microcontrolador (Esp32);
-Construir uma interface dashboard através do Node-RED para que o usuário possa realizar o controle e visualizar os dados pela mesma;
-Integrar o banco de dados RDS no Node-RED;
-Desenvolver o circuito e integrar o Esp32;
-Aplicar o sistema na residência.

Esse trabalho é um complemento ao TCC de Pedro Eduardo Suga Machado sobre "Automação residencial visando um melhor gerenciamento energético".

Diagrama esquemático do controle do relay:

Diagrama esquemático da coleta de dados do sensor:

Dashboard desenvolvida no Node-RED:
