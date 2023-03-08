

function LoadCalibration()
 {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "LoadCalibration", true);
    xhttp.send();
}//---------------------------------------


//-----------cherche l'info allumage rampe-----------------
$(document).ready(function(){
    $("#appliquerRampeOn").click(function(){
        var valeur = $("#choixHeureRampeOn").val();
        $.post("heureRampeOn",{
            valeurHeureRampeOn: valeur
        });
    });
});


//----------cherche l'info extinction rampe------------
$(document).ready(function(){
    $("#appliquerRampeOff").click(function(){
        var valeur = $("#choixHeureRampeOff").val();
        $.post("heureRampeOff",{
            valeurHeureRampeOff: valeur
        });
    });
});

//---change la vitesse du ventilateur de rampe--------
$(document).ready(function(){
    $("#appliquerVitesseVentilation").click(function(){
        var valeur = $("#choixVitesseVentilation").val();
        $.post("vitesseVentilation",{
            valeurVitesseVentilation: valeur
        });
    });
});

//--------------------------------------

setInterval(function getData()
{

// transfert la valeur du courant
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("valeurSSID").innerHTML = this.responseText;
        }
    };	
    xhttp.open("GET", "lireSSID", true);
    xhttp.send();
	
// transfert la valeur du courant

	var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("valeurCourant").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "lireCourant", true);
    xhttp.send();
//--------------------------------------------		    
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("valeurPuissance").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "lirePuissance", true);
    xhttp.send();
//--------------------------------------------    
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("valeurRSSI").innerHTML = this.responseText;
        }
    };	
    xhttp.open("GET", "lireNiveauRSSI", true);
    xhttp.send();
//---------------------------------------------------------   
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("valeurBatterie").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "lireBatterie", true);
    xhttp.send();
    
//----------------------------------------------------------   
	var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("valeurDistance").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "lireDistance", true);
    xhttp.send();
		
	
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("valeurHumidite").innerHTML = this.responseText;
        }
    };	
    xhttp.open("GET", "lireHumidite", true);
    xhttp.send();
		
	
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("valeurTemperature").innerHTML = this.responseText;
        }
    };	
    xhttp.open("GET", "lireTemperature", true);
    xhttp.send();

    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("valeurPression").innerHTML = this.responseText;
        }
    };	
    xhttp.open("GET", "lirePression", true);
    xhttp.send();
	
    
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("valeurTemperatureDS18S20").innerHTML = this.responseText;
        }
    };	
    xhttp.open("GET", "LireTemperatureDS18S20", true);
    xhttp.send();
    

	var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("valeurCourantAh").innerHTML = this.responseText;
        }
    };	
    xhttp.open("GET", "lireCourantAh", true);
    xhttp.send();	
	
	
//    var xhttp = new XMLHttpRequest();
//    xhttp.onreadystatechange = function()
//    {
//        if(this.readyState == 4 && this.status == 200)
//        {
//            document.getElementById("valeurPoids1").innerHTML = this.responseText;
//        }
//    };	
//    xhttp.open("GET", "lirePoids1", true);
//    xhttp.send();
    
    
//    var xhttp = new XMLHttpRequest();
//    xhttp.onreadystatechange = function()
//    {
//        if(this.readyState == 4 && this.status == 200)
//        {
//            document.getElementById("valeurPoids2").innerHTML = this.responseText;
//        }
//    };	
//    xhttp.open("GET", "lirePoids2", true);
//    xhttp.send();
    
    
//    var xhttp = new XMLHttpRequest();
//    xhttp.onreadystatechange = function()
//    {
//        if(this.readyState == 4 && this.status == 200)
//        {
//            document.getElementById("valeurPoids3").innerHTML = this.responseText;
//        }
//    };	
//    xhttp.open("GET", "lirePoids3", true);
//    xhttp.send();
    
//    var xhttp = new XMLHttpRequest();
//    xhttp.onreadystatechange = function()
//    {
//        if(this.readyState == 4 && this.status == 200)
//        {
//            document.getElementById("valeurPoids4").innerHTML = this.responseText;
//        }
//    };	
//    xhttp.open("GET", "lirePoids4", true);
//    xhttp.send();
    
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("valeurPoidsTotal").innerHTML = this.responseText;
        }
    };	
    xhttp.open("GET", "lirePoidsTotal", true);
    xhttp.send(); 
    
    
	var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("valeurTimeLightOn").innerHTML = this.responseText;
        }
    };	
    xhttp.open("GET", "lireTimeLightOn", true);
    xhttp.send();	
	
    
	var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("valeurTimeLightOff").innerHTML = this.responseText;
        }
    };	
    xhttp.open("GET", "lireTimeLightOff", true);
    xhttp.send()	
	
	
}, 5000);