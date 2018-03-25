exports.handler = function (request, context) {
    if (request.directive.header.namespace === 'Alexa.Discovery' && request.directive.header.name === 'Discover') {
        log("DEBUG:", "Discover request",  JSON.stringify(request));
        handleDiscovery(request, context, "");
    }

    else if(request.directive.header.namespace === 'Alexa.PercentageController' && request.directive.header.name === 'SetPercentage') {
        log("DEBUG:", "Percentage request",  JSON.stringify(request));
        handlePercentage(request, context, "");
    }

    else{
        log("DEBUG:", "Someother Request", JSON.stringify(request));
    }


    function handlePercentage(request, context){
     var contextResult = {
            "properties": [
                {
                    "namespace": "Alexa.PercentageController",
                    "name": "percentage",
                    "value": 74,
                    "timeOfSample": new Date().toISOString(),
                    "uncertaintyInMilliseconds": 200
                },
                {
                    "namespace": "Alexa.EndpointHealth",
                    "name": "connectivity",
                    "value": {
                        "value": "OK"
                    },
                    "timeOfSample": new Date().toISOString(),
                    "uncertaintyInMilliseconds": 200
                }
            ]

            };

            var responseHeader = request.directive.header;
            responseHeader.namespace = "Alexa";
            responseHeader.name = "Response";
            responseHeader.messageId = responseHeader.messageId + "-R";
            var response = {
                context: contextResult,
                event: {
                    header: responseHeader,
                    endpoint: request.directive.endpoint
                },
                payload: {}

            };
            log("DEBUG", "Alexa.Percentage ", JSON.stringify(response));

            var url = 'http://blinds-01.in.aijay.id.au/' + request.directive.payload.percentage;

            const https = require('https');
            https.get(url, function(res) {
                console.log("Got response: " + res.statusCode);
                context.succeed(response);
            }).on('error', function(e) {
            console.log("Got error: " + e.message);
            context.succeed(response);
          });
    }

    function handleDiscovery(request, context) {
        var payload = {
            "endpoints":
            [
                {
                    "endpointId": "waveb_blinds",
                    "manufacturerName": "waveB",
                    "friendlyName": "Blinds",
                    "description": "WaveB Blinds",
                    "displayCategories": ["OTHER"],
                    "capabilities":
                    [
                        {

                            "type": "AlexaInterface",
                            "interface": "Alexa.PercentageController",
                            "version": "3",
                            "properties": {
                                "supported": [
                                    {
                                        "name": "percentage"
                                    }
                                ],
                                "retrievable": true
                            }

                        }
                    ]
                }
            ]
        };
        var header = request.directive.header;
        header.name = "Discover.Response";
        log("DEBUG", "Discovery Response: ", JSON.stringify({ header: header, payload: payload }));
        context.succeed({ event: { header: header, payload: payload } });
    }

    function log(message, message1, message2) {
        console.log(message + message1 + message2);
    }
};
