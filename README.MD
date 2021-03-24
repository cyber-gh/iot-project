<h2>How to run</h2>

<pre>docker-compose up -d --build</pre> or 
<pre>sudo docker-compose up -d --build</pre>
<br>
<pre>curl http://localhost:9080/products</pre>

API Documentation
https://app.swaggerhub.com/apis-docs/dioji/smart-fridge/1.0.0

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
mosquitto_pub -p 1882 -m "message from mosquitto_pub client" -t "fridge_commands"

mosquitto_sub -t "status" -q 1


<h2>API</h2>
<pre>http://localhost:9080/fridge/recommendProduct/ --recommend products with min expire date
http://localhost:9080/fridge/recommendProduct/Mere/Pere -- if this products are in fridge:
                                                           recommend product with min expire date
                                                        else:
                                                           recommend most similiar product by name with min expire date
                                                           
http://localhost:9080/fridge/temperature/ -- get current temperature     

http://localhost:9080/fridge/temperature/:temp -- set temperature as temp

mosquitto_pub -p 1882 -m "{\"date\":\"2021-03-23\",\"id\":1,\"max_temp\":20,\"name\":\"test\",\"quantity\":3}" -t "fridge_commands" - add product

mosquitto_pub -p 1882 -m "{\"status\":\"SETFRIDGE\", \"value\": \"25\"}" -t "fridge_commands" - set fridge temperature

mosquitto_pub -p 1882 -m "{\"productName\":\"Mere\", \"quantity\": 25}" -t "fridge_commands" - increase product quantity

mosquitto_pub -p 1882 -m "{\"status\": \"DELETE\", \"value\": \"Mere\"}" -t "fridge_commands" - Delete Mere objectt

</pre>    
