setInterval(function () {
            var zapytanie = new XMLHttpRequest();
                        zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("temp1_nastawa").innerHTML = this.responseText + "°C";
                }
            };
			zapytanie.open("GET", "/webTemp1",true);
            zapytanie.send();
        }, 300);
		
			document.getElementById("zezwolenie").onclick = function () { 
		var zapytanie = new XMLHttpRequest();            //wyślijmy zapytanie GET, pod adresem /on      
         zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                   document.getElementById("zezwolenie").className = "btn_on";//czerwony
                }
				if (this.readyState == 4 && this.status == 210) {
                   document.getElementById("zezwolenie").className = "btn_off";//zielony
                }
				
		}
		zapytanie.open("GET", "/autoManual",true);
        zapytanie.send();
	};

setInterval(function () {
            var zapytanie = new XMLHttpRequest();
                       zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("temp_zew").innerHTML = this.responseText ;
                }
            };
			 zapytanie.open("GET", "/tempOut",true);
            zapytanie.send();
        }, 2909);
		
		setInterval(function () {
            var zapytanie = new XMLHttpRequest();
            
            zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("wilg_zew").innerHTML = this.responseText ;
                }
            };
			zapytanie.open("GET", "/hudmiOut",true);
            zapytanie.send();
        }, 3009);
		
setInterval(function () {
            var zapytanie = new XMLHttpRequest();
            
            zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("podlogowka1").className = "blink_1";
					document.getElementById("podlogowka1Wlacz").className = "btn_heat";
					
                }
				if (this.readyState == 4 && this.status == 210) {
                    document.getElementById("podlogowka1").className = "";
					document.getElementById("podlogowka1Wlacz").className = "btn_no_heat";
					
                }
            };
			zapytanie.open("GET", "/heatOut1",true);
            zapytanie.send();
        }, 260);		
		
		setInterval(function () {
            var zapytanie = new XMLHttpRequest();
            
            zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("podlogowka2").className = "blink_1";
					document.getElementById("podlogowka2Wlacz").className = "btn_heat";
                }
				if (this.readyState == 4 && this.status == 210) {
                    document.getElementById("podlogowka2").className = "";
					document.getElementById("podlogowka2Wlacz").className = "btn_no_heat";
                }
            };
			zapytanie.open("GET", "/heatOut2",true);
            zapytanie.send();
			}, 250);
			
			setInterval(function () {
            var zapytanie = new XMLHttpRequest();
            
            zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                   document.getElementById("zezwolenie").className = "btn_on";//czerwony
                }
				if (this.readyState == 4 && this.status == 210) {
                   document.getElementById("zezwolenie").className = "btn_off";//zielony
                }
				
            };
			zapytanie.open("GET", "/refresh",true);
            zapytanie.send();
			}, 270);
			
			document.getElementById("podlogowka1Wlacz").onclick = function () { 
		var zapytanie = new XMLHttpRequest();            //wyślijmy zapytanie GET, pod adresem /on      
         zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                   document.getElementById("podlogowka1Wlacz").className = "btn_no_heat";//czerwony
                }
				if (this.readyState == 4 && this.status == 210) {
                   document.getElementById("podlogowka1Wlacz").className = "btn_heat";//zielony
                }
				
		}
		zapytanie.open("GET", "/onOff1",true);
        zapytanie.send();
	};
	
	document.getElementById("podlogowka2Wlacz").onclick = function () { 
		var zapytanie = new XMLHttpRequest();            //wyślijmy zapytanie GET, pod adresem /on      
         zapytanie.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                   document.getElementById("podlogowka2Wlacz").className = "btn_no_heat";//czerwony
                }
				if (this.readyState == 4 && this.status == 210) {
                   document.getElementById("podlogowka2Wlacz").className = "btn_heat";//zielony
                }
				
		}
		zapytanie.open("GET", "/onOff2",true);
        zapytanie.send();
	};