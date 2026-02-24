#!/bin/bash

date=$(date +"%y-%m-%d")
data=$(curl -s wttr.in/hyderabad?format=j1 | jq -r '
.weather[0].hourly[3] |
[ .tempC, .FeelsLikeC, .humidity, .weatherDesc[0].value, .windspeedKmph ] | @tsv
')
echo -e "$date\t$data" >> weather_data.log
