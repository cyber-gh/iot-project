<h3>Prerequisites</h3>
<ul>
    <li>Docker</li>
    <li>docker-compose</li>
</ul>

<h2>How to run</h2>

<pre>sudo docker-compose up -d --build</pre> 

Alternatively, if you're on an ubuntu system, run all the commands from the Dockerfile step by step

<h4> Analysis report can be found in the documentation folder</h4>
<h3>Full API Documentation</h3>
https://app.swaggerhub.com/apis-docs/dioji/smart-fridge/1.0.0

<h3>Example request </h3>
<pre>curl http://localhost:9080/products</pre>



<h3> Developer Resources</h3>

JSON Usage
https://kezunlin.me/post/f3c3eb8/ <br>

Sqlite usage
https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.
htm

MQTT Usage
https://github.com/eclipse/paho.mqtt.cpp

MQTT Examples
https://github.com/eclipse/paho.mqtt.cpp/tree/master/src/samples


<h2>MQTT API</h2>

```

mosquitto_pub -p 1882 -m "{\"date\":\"2021-03-23\",\"id\":1,\"max_temp\":20,\"name\":\"test\",\"quantity\":3}" -t "fridge_commands" - add product

mosquitto_pub -p 1882 -m "{\"status\":\"SETFRIDGE\", \"value\": \"25\"}" -t "fridge_commands" - set fridge temperature

mosquitto_pub -p 1882 -m "{\"productName\":\"Mere\", \"quantity\": 25}" -t "fridge_commands" - increase product quantity

mosquitto_pub -p 1882 -m "{\"status\": \"DELETE\", \"value\": \"Mere\"}" -t "fridge_commands" - Delete Mere object

mosquitto_pub -p 1882 -m 75 -t "voltage" - send current voltage.

mosquitto_sub -p 1882 -t "status" -q 1


```  

TO TEST API

curl -X GET "http://localhost:9080/products" -H  "accept: application/json"

curl -X POST "http://localhost:9080/products" -H  "accept: application/json" -H  "Content-Type: application/json" -d "{  \"id\": 99,  \"name\": \"oua\",  \"quantity\": 2,  \"date\": \"2020-09-09\",  \"max_temp\": -10}"

curl -X GET "http://localhost:9080/products/oua" -H  "accept: application/json"

curl -X GET "http://localhost:9080/products/3" -H  "accept: application/json" 

curl -X GET "http://localhost:9080/products/2021-03-20" -H  "accept: application/json" 

curl -X DELETE "http://localhost:9080/products/oua" -H  "accept: application/json" 

curl -X GET "http://localhost:9080/fridge/temperature" -H  "accept: application/json"

curl -X POST "http://localhost:9080/fridge/temperature/-90" -H  "accept: application/json" -d ""

curl -X GET "http://localhost:9080/fridge/recommendProduct/Mere/Oua" -H  "accept: application/json"

curl -X PUT "http://localhost:9080/fridge/eco" -H  "accept: application/json"
