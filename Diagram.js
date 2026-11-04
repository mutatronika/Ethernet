flowchart TD

subgraph group_core["Embedded practice suites"]
  node_sensorica["Sensorica<br/>arduino sample<br/>[Sensorica.ino]"]
  node_sensorica_ui["Sensorica UI<br/>web client<br/>[index.html]"]
  node_practica4_ajax["SD Ajax<br/>arduino web server"]
  node_practica4_xml["XML Ajax<br/>arduino web server"]
  node_practica4_gauge["Gauge UI<br/>arduino dashboard"]
  node_practica4_image["SD Image<br/>arduino web server"]
  node_practica5_panel["Solar Panel<br/>arduino monitor"]
  node_practica5_io["IO Server<br/>arduino web server"]
  node_practica5_mega["Mega Ajax<br/>arduino web server"]
  node_rgb_web["RGB Web<br/>modular firmware<br/>[RGB_web.ino]"]
  node_dual_dht["Dual DHT<br/>thermostat sketch"]
  node_simple_weather["Weather Station<br/>monitoring sketch"]
end

subgraph group_integrations["External integrations"]
  node_prac6_thingspeak["ThingSpeak<br/>cloud telemetry"]
  node_dht_mysql["DHT MySQL<br/>php-backed logger"]
  node_w5100_eventos["Eventos<br/>arduino php app<br/>[W5100-Eventos.ino]"]
  node_arduino_voice["Voice Control<br/>arduino php app"]
  node_arduino_sms["SMS Web<br/>arduino integration<br/>[WebSMS.ino]"]
end

subgraph group_docs["Documentation"]
  node_guias["Guides<br/>support docs"]
end

subgraph group_side["Side projects"]
  node_alarmas_ut["AlarmasUT<br/>node web app<br/>[server.js]"]
end

subgraph group_hardware["Hardware references"]
  node_documentacion["References<br/>support docs"]
  node_w5100_refs["W5100 Kit<br/>hardware refs<br/>[W5100.pdf]"]
end

node_root_readme["Repo README<br/>overview<br/>[README.md]"]

node_root_readme -->|"points to"| node_sensorica
node_sensorica -->|"serves"| node_sensorica_ui
node_sensorica -.->|"same pattern"| node_practica4_ajax
node_practica4_ajax -.->|"variant of"| node_practica4_xml
node_practica4_ajax -.->|"variant of"| node_practica4_gauge
node_practica4_ajax -.->|"variant of"| node_practica4_image
node_practica4_ajax -.->|"evolves into"| node_practica5_io
node_practica5_io -.->|"evolves into"| node_practica5_panel
node_practica5_io -.->|"board variant"| node_practica5_mega
node_practica5_panel -.->|"cloud path"| node_prac6_thingspeak
node_sensorica -.->|"sensor family"| node_dual_dht
node_sensorica -.->|"sensor family"| node_simple_weather
node_rgb_web -.->|"modular control"| node_arduino_voice
node_arduino_voice -.->|"external backend"| node_dht_mysql
node_dht_mysql -.->|"php integration"| node_w5100_eventos
node_w5100_eventos -.->|"service integration"| node_arduino_sms
node_alarmas_ut -.->|"separate project"| node_root_readme
node_guias -.->|"documents"| node_sensorica
node_documentacion -.->|"supports"| node_w5100_refs
node_w5100_refs -.->|"enables"| node_practica4_ajax

click node_root_readme "https://github.com/pr3t3r1t0/ethernet/blob/main/README.md"
click node_sensorica "https://github.com/pr3t3r1t0/ethernet/blob/main/Sensorica/Sensorica.ino"
click node_sensorica_ui "https://github.com/pr3t3r1t0/ethernet/blob/main/Sensorica/index.html"
click node_practica4_ajax "https://github.com/pr3t3r1t0/ethernet/blob/main/Practica4-eth_websrv_SD_Ajax/Practica4-eth_websrv_SD_Ajax.ino"
click node_practica4_xml "https://github.com/pr3t3r1t0/ethernet/blob/main/Practica4-eth_websrv_SD_Ajax_XML/Practica4-eth_websrv_SD_Ajax_XML.ino"
click node_practica4_gauge "https://github.com/pr3t3r1t0/ethernet/blob/main/Practica4-eth_websrv_SD_Ajax_gauge/Practica4-eth_websrv_SD_Ajax_gauge.ino"
click node_practica4_image "https://github.com/pr3t3r1t0/ethernet/blob/main/Practica4-eth_websrv_SD_image/Practica4-eth_websrv_SD_image.ino"
click node_practica5_panel "https://github.com/pr3t3r1t0/ethernet/blob/main/Practica5-web_server_Panel/PanelSolarsimple/PanelSolarsimple.ino"
click node_practica5_io "https://github.com/pr3t3r1t0/ethernet/blob/main/Practica5-web_server_IO/eth_websrv_SD_Ajax_in_out/eth_websrv_SD_Ajax_in_out.ino"
click node_practica5_mega "https://github.com/pr3t3r1t0/ethernet/blob/main/Practica5-Mega/eth_websrv_SD_Ajax_24_out/eth_websrv_SD_Ajax_24_out.ino"
click node_prac6_thingspeak "https://github.com/pr3t3r1t0/ethernet/blob/main/Practica6-ThingSpeak/Practica6-ThingSpeak.ino"
click node_dht_mysql "https://github.com/pr3t3r1t0/ethernet/blob/main/arduino-dht-ethernet-master/Arduino sketch/DHT_W5100_logger.ino"
click node_w5100_eventos "https://github.com/pr3t3r1t0/ethernet/blob/main/W5100-Eventos-master/W5100-Eventos.ino"
click node_arduino_voice "https://github.com/pr3t3r1t0/ethernet/blob/main/Arduino-Voice-control-master/arduino_english_version.ino"
click node_arduino_sms "https://github.com/pr3t3r1t0/ethernet/blob/main/arduino-sms-web-master/WebSMS.ino"
click node_rgb_web "https://github.com/pr3t3r1t0/ethernet/blob/main/ServidorWebArduino-controlar_LedRGB-master/RGB_web/RGB_web.ino"
click node_dual_dht "https://github.com/pr3t3r1t0/ethernet/blob/main/DualDHTthermostat-master/DualDHTthermostat.ino"
click node_simple_weather "https://github.com/pr3t3r1t0/ethernet/blob/main/simpleweatherstation-master/simpleweatherstation.ino"
click node_alarmas_ut "https://github.com/pr3t3r1t0/ethernet/blob/main/AlarmasUT-main/server.js"
click node_guias "https://github.com/pr3t3r1t0/ethernet/blob/main/Guias/Resumen_Practicas.md"
click node_documentacion "https://github.com/pr3t3r1t0/ethernet/blob/main/Documentacion/W5100_Datasheet_v1_1_6.pdf"
click node_w5100_refs "https://github.com/pr3t3r1t0/ethernet/blob/main/W5100-master/W5100.pdf"

classDef toneNeutral fill:#f8fafc,stroke:#334155,stroke-width:1.5px,color:#0f172a
classDef toneBlue fill:#dbeafe,stroke:#2563eb,stroke-width:1.5px,color:#172554
classDef toneAmber fill:#fef3c7,stroke:#d97706,stroke-width:1.5px,color:#78350f
classDef toneMint fill:#dcfce7,stroke:#16a34a,stroke-width:1.5px,color:#14532d
classDef toneRose fill:#ffe4e6,stroke:#e11d48,stroke-width:1.5px,color:#881337
classDef toneIndigo fill:#e0e7ff,stroke:#4f46e5,stroke-width:1.5px,color:#312e81
classDef toneTeal fill:#ccfbf1,stroke:#0f766e,stroke-width:1.5px,color:#134e4a
class node_sensorica,node_sensorica_ui,node_practica4_ajax,node_practica4_xml,node_practica4_gauge,node_practica4_image,node_practica5_panel,node_practica5_io,node_practica5_mega,node_rgb_web,node_dual_dht,node_simple_weather toneBlue
class node_prac6_thingspeak,node_dht_mysql,node_w5100_eventos,node_arduino_voice,node_arduino_sms toneAmber
class node_guias toneMint
class node_alarmas_ut toneRose
class node_documentacion,node_w5100_refs toneIndigo
class node_root_readme toneNeutral
