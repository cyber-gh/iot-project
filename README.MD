<h2>How to run</h2>

<pre>docker-compose up -d --build</pre> or 
<pre>sudo docker-compose up -d --build</pre>
<br>
<pre>curl http://localhost:9080/products</pre>

JSON Usage
https://kezunlin.me/post/f3c3eb8/ <br>

Sqlite usage
https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.
htm

MQTT Usage
https://github.com/eclipse/paho.mqtt.cpp

MQTT Examples
https://github.com/eclipse/paho.mqtt.cpp/tree/master/src/samples

How to test mqtt <br>
mosquitto_pub -m "message from mosquitto_pub client" -t "hello"

<h2>API</h2>
<pre>http://localhost:9080/fridge/recommendProduct/ --recommend products with min expire date
http://localhost:9080/fridge/recommendProduct/Mere/Pere -- if this products are in fridge:
                                                           recommend product with min expire date
                                                        else:
                                                           recommend most similiar product by name with min expire date
</pre>    
