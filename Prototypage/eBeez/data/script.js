$(document).ready(function() {
    affichage();
	$("#appliquer-params").click(function(){
        var valLog = $("#login").val();
		var valPass1 = $("#pass1-log").val();
		var valPass2 = $("#pass2-log").val();
		var valRuche = $("#nomruche").val();
		var valMVeille = $("#mveille").is(":checked");
		var valtpsVeille = $("#tmpveille").val();
		if (valPass1==valPass2) {
			$.post("params",
			{
				webLogin: valLog,
				webPass: valPass1,
				rucheName: valRuche,
				ModeVeille: valMVeille,
				iTimeToSleep: valtpsVeille
			});
		} else {
			alert("Les deux password sont différents");
		}

    });
	$("#appliquer-wifi").click(function(){
		var valModeWifi = $("#choixModeWifi").val();
		var valSSID_AP = $("#ssid-ap").val();
		var valPass1_AP = $("#pass1-ap").val();
		var valPass2_AP = $("#pass2-ap").val();
		var valSSID_Client = $("#ssid-client").val();
		var valPass1_Client = $("#pass1-client").val();
		var valPass2_Client = $("#pass2-client").val();
        if (valPass1_AP==valPass2_AP && valPass1_Client==valPass2_Client) {
			$.post("wifi",{
				wifi_mode: valModeWifi,
				ssid_ap: valSSID_AP,
				pass_ap: valPass1_AP,
				ssid_client: valSSID_Client,
				pass_client: valPass1_Client
			});
		} else {
			if (valPass1_AP!=valPass2_AP) {alert("Les deux password AP sont différents");}
			if (valPass1_Client!=valPass2_Client) {alert("Les deux password CLIENT sont différents");}
		}
    });
	
	$("#appliquer-mqtt").click(function(){
		var mqtt_serveur = $("#mqtt_serveur").val();
		var mqtt_port = $("#mqtt_port").val();
		var mqtt_user = $("#mqtt_user").val();
		var mqtt_pass1 = $("#mqtt_pass1").val();
		var mqtt_pass2 = $("#mqtt_pass2").val();
        if (mqtt_pass1==mqtt_pass2) {
			$.post("mqtt",{
				mqtt_serveur: mqtt_serveur,
				mqtt_port: mqtt_port,
				mqtt_user: mqtt_user,
				mqtt_pass: mqtt_pass1
			});
		} else {
			if (mqtt_pass1!=mqtt_pass2) {alert("Les deux password sont différents");}
		}
    });
	
	$("#calibrer").click(function(){
		var f_Ajust = $("#fAjust").val();
		alert(f_Ajust);
        $.post("calib",{
			fAjustement: f_Ajust
		});
    });
});

function affichage() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () { 
        if (this.readyState == 4 && this.status == 200) {
			const valeurs = this.responseText.split('&');
            document.getElementById("login").value = valeurs[0];
			document.getElementById("pass1-log").value = valeurs[1];
			document.getElementById("pass2-log").value = valeurs[1];
			document.getElementById("nomruche").value = valeurs[2];
			document.getElementById("mveille").value = valeurs[3];
			document.getElementById("tmpveille").value = valeurs[4];
			document.getElementById("choixModeWifi").value = valeurs[5];
			document.getElementById("ssid-ap").value = valeurs[6];
			document.getElementById("pass1-ap").value = valeurs[7];
			document.getElementById("pass2-ap").value = valeurs[7];
			document.getElementById("ssid-client").value = valeurs[8];
			document.getElementById("pass1-client").value = valeurs[9];
			document.getElementById("pass2-client").value = valeurs[9];
			document.getElementById("fAjust").value = valeurs[10];
			document.getElementById("cal1").value = valeurs[11];
			document.getElementById("cal2").value = valeurs[12];
			document.getElementById("cal3").value = valeurs[13];
			document.getElementById("cal4").value = valeurs[14];
			document.getElementById("mqtt_serveur").value = valeurs[15];
			document.getElementById("mqtt_port").value = valeurs[16];
			document.getElementById("mqtt_user").value = valeurs[17];
			document.getElementById("mqtt_pass1").value = valeurs[18];
			document.getElementById("mqtt_pass2").value = valeurs[18];
        }
    };

    xhttp.open("GET", "getData", true);
    xhttp.send();
}

function rebootButton() {
	alert("reboot");
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "reboot", true);
    xhttp.send();
}			