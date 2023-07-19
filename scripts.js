document.getElementById("on").onclick = function () {  //po nacisinięciu elementu o ID "on"
        var zapytanie = new XMLHttpRequest();            //wyślijmy zapytanie GET, pod adresem /on      
        zapytanie.open("GET", "/on",true);
        zapytanie.send();                               
    };

    document.getElementById("off").onclick = function () {  //po nacisinięciu elementu o ID "off"
        var zapytanie = new XMLHttpRequest();             //wyślijmy zapytanie GET, pod adresem /off   
        zapytanie.open("GET", "/off",true);
        zapytanie.send();                               
    };
	
	 setInterval(function () {
            var zapytanie = new XMLHttpRequest();
                        zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("pomiar").innerHTML ="Pomiar test:"+ this.responseText;
                }
            };
			zapytanie.open("GET", "/adc",true);
            zapytanie.send();
        }, 10000);
		
		setInterval(function () {
            var zapytanie = new XMLHttpRequest();
            
            zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("pomiar1").innerHTML = this.responseText ;
                }
            };
			zapytanie.open("GET", "/dht1",true);
            zapytanie.send();
        }, 2009);
		
		setInterval(function () {
            var zapytanie = new XMLHttpRequest();
            
            zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("pomiar2").innerHTML = this.responseText ;
                }
            };
			zapytanie.open("GET", "/dht2",true);
            zapytanie.send();
        }, 2109);
		
		setInterval(function () {
            var zapytanie = new XMLHttpRequest();
           
            zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("data").innerHTML = this.responseText;
                }
            };
			 zapytanie.open("GET", "/data",true);
            zapytanie.send();
        }, 1010);
		
		setInterval(function () {
            var zapytanie = new XMLHttpRequest();
            
            zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("czas").innerHTML = this.responseText;
                }
            };
			zapytanie.open("GET", "/czas",true);
            zapytanie.send();
        }, 1020);
		
		setInterval(function () {
            var zapytanie = new XMLHttpRequest();
            
            zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("czasNtp").innerHTML = "czas Ntp: " + this.responseText;
                }
            };
			zapytanie.open("GET", "/czasNtp",true);
            zapytanie.send();
        }, 1030);
		
		setInterval(function () {
            var zapytanie = new XMLHttpRequest();
            
            zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("pomiarWilg1").innerHTML = + this.responseText;
                }
            };
			zapytanie.open("GET", "/hudmi1",true);
            zapytanie.send();
        }, 2209);
		
		setInterval(function () {
           var zapytanie = new XMLHttpRequest();
            
            zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("pomiarWilg2").innerHTML = this.responseText;
                }
            };
			 zapytanie.open("GET", "/hudmi2",true);
			 zapytanie.send();
        }, 2309);
		
		setInterval(function () {
            var zapytanie = new XMLHttpRequest();
            
            zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("apiStacja").innerHTML =  this.responseText;
                }
            };
			zapytanie.open("GET", "/apiStation",true);
			zapytanie.send();
        }, 2409);

    setInterval(function () {
            var zapytanie = new XMLHttpRequest();
            
            zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("apiPressure").innerHTML = "Ciśnienie atmosferyczne: " + this.responseText;
                }
            };
			zapytanie.open("GET", "/apiPressure",true);
            zapytanie.send();
        }, 2709);
				
				setInterval(function () {
            var zapytanie = new XMLHttpRequest();
           
            zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("daneIp").innerHTML = "IP: " + this.responseText;
                }
            };
			 zapytanie.open("GET", "/ethernetInfo",true);
            zapytanie.send();
        }, 2809);
		
		document.getElementById("bt_up1").onclick = function () {  //po nacisinięciu elementu o ID "off"
            var zapytanie = new XMLHttpRequest();             //wyślijmy zapytanie GET, pod adresem /off   
            zapytanie.open("GET", "/button1Up",true);
            zapytanie.send();   
        };
        
            document.getElementById("bt_dw1").onclick = function () {  //po nacisinięciu elementu o ID "off"
            var zapytanie = new XMLHttpRequest();             //wyślijmy zapytanie GET, pod adresem /off   
            zapytanie.open("GET", "/button1Down",true);
            zapytanie.send();   
        };
    
        document.getElementById("bt_up2").onclick = function () {  //po nacisinięciu elementu o ID "off"
            var zapytanie = new XMLHttpRequest();             //wyślijmy zapytanie GET, pod adresem /off   
            zapytanie.open("GET", "/button2Up",true);
            zapytanie.send();   
        };
    
        document.getElementById("bt_dw2").onclick = function () {  //po nacisinięciu elementu o ID "off"
            var zapytanie = new XMLHttpRequest();             //wyślijmy zapytanie GET, pod adresem /off   
            zapytanie.open("GET", "/button2Down",true);
            zapytanie.send();   
        };
    
    setInterval(function () {
                var zapytanie = new XMLHttpRequest();
                
                zapytanie.onreadystatechange = function () {
                    if (this.readyState == 4 && this.status == 200) {
                        document.getElementById("temp2_nastawa").innerHTML = this.responseText + "°C";
                    }
                };
				zapytanie.open("GET", "/webTemp2",true);
                zapytanie.send();
            }, 320);
		