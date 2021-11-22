
const char HOME_WIFI_SET_PAGE[] = R"=====(
<html><head><title>ESP WIFI 配网</title>
<meta http-equiv='Content-Type' content='text/html; charset=UTF-8'>
<meta name='viewport' content='width=device-width, initial-scale=1.0, user-scalable=no, minimum-scale=1.0, maximum-scale=1.0'/>
<style type='text/css'>
.login-page {
  width: 360px;
  padding: 8% 0 0;
  margin: auto;}
.form {
  position: relative;
  z-index: 1;
  background: #FFFFFF;
  max-width: 360px;
  margin: 0 auto 100px;
  padding: 45px;
  text-align: center;
  box-shadow: 0 0 20px 0 rgba(0, 0, 0, 0.2), 0 5px 5px 0 rgba(0, 0, 0, 0.24);}
.form input {
  font-family: 'Roboto', sans-serif;
  outline: 0;
  background: #f2f2f2;
  width: 100%;
  border: 0;
  margin: 0 0 15px;
  padding: 15px;
  box-sizing: border-box;
  font-size: 14px;}    
.form button {
  font-family: 'Roboto', sans-serif;
  text-transform: uppercase;
  outline: 0;
  background: #4CAF50;
  width: 100%;
  border: 0;
  padding: 15px;
  color: #FFFFFF;
  font-size: 14px;
  -webkit-transition: all 0.3 ease;
  transition: all 0.3 ease;
  cursor: pointer;}
.form button:hover,.form button:active,.form button:focus {
  background: #43A047;}
.form .message {
  margin: 15px 0 0;
  color: #b3b3b3;
  font-size: 12px;} 
.form .msgshow {
  margin: 0 0 13px;
  color: #b3b3b3;
  font-size: 12px;}
body {
  background: #76b852;
  background: -webkit-linear-gradient(right, #76b852, #8DC26F);
  background: -moz-linear-gradient(right, #76b852, #8DC26F);
  background: -o-linear-gradient(right, #76b852, #8DC26F);
  background: linear-gradient(to left, #76b852, #8DC26F);
  font-family: 'Roboto', sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;}
</style><script>
function sendSetWifi() {
  var name = document.getElementById('name').value;
  var pass = document.getElementById('pass').value;
  if(name == ''){window.alert('请输入正确的WIFI名称!!!');return false;}
  if(pass == ''){if(!confirm('WIFI密码未输入,如果连接的WIFI没有密码请点击确定,否则点击取消继续输入密码,确定要继续吗?')){return false;}}
  document.getElementById("show").innerHTML = "";
  document.getElementById("save").disabled = true;
  var xhttp = new XMLHttpRequest();
  
  xhttp.onreadystatechange = function() {
	document.getElementById("save").disabled = false;
    if (this.readyState == 4 && this.status == 200) {	
		if(this.responseText == "OK"){
			document.getElementById("show").innerHTML = "网络配置成功,模块将自动重启尝试连接到WIFI网络";
      window.opener = null; 
      window.open("", "_self", "");
      window.alert(document.getElementById("show").innerHTML);
      window.close();
		}else{
			document.getElementById("show").innerHTML = "网络配置失败,保存WIFI连接信息,遇到未知错误";
      window.alert(document.getElementById("show").innerHTML);
		}
    }
  };
  
  xhttp.onerror = function() {
	document.getElementById("show").innerHTML = "网络配置失败,芯片未响应请求";
	document.getElementById("save").disabled = false;
  };
  
  xhttp.open("GET", "/SAVEWIFI?name=" + name + "&pass=" + pass, true);
  xhttp.send();
  document.getElementById("show").innerHTML = "WIFI 配置请求已发送, 正在等待配置状态";
}
</script></head><body><div class='login-page'>
    <div class='form'>
	<p id='show' class='msgshow'></p>
	<input id='name' placeholder='WIFI 名称'>
    <input type='password' id='pass' placeholder='WIFI 密码'>
    <button id='save' onclick='javascript:sendSetWifi();'>连接 WIFI</button>
	<p class='message'>如果连接的 WIFI 无密码, 请留空即可</p>
    </div></div>
</body></html>
)=====";

const char TARND_SET_PAGE[] = R"=====(
<html><head><title>数据透传服务端配置</title>
<meta http-equiv='Content-Type' content='text/html; charset=UTF-8'>
<meta name='viewport' content='width=device-width, initial-scale=1.0, user-scalable=no, minimum-scale=1.0, maximum-scale=1.0'/>

<style type='text/css'>
.login-page {
  width: 360px;
  padding: 8% 0 0;
  margin: auto;}
.form {
  position: relative;
  z-index: 1;
  background: #FFFFFF;
  max-width: 360px;
  margin: 0 auto 100px;
  padding: 45px;
  text-align: center;
  box-shadow: 0 0 20px 0 rgba(0, 0, 0, 0.2), 0 5px 5px 0 rgba(0, 0, 0, 0.24);}
.form input {
  font-family: 'Roboto', sans-serif;
  outline: 0;
  background: #f2f2f2;
  width: 100%;
  border: 0;
  margin: 0 0 15px;
  padding: 15px;
  box-sizing: border-box;
  font-size: 14px;}
.form button {
  font-family: 'Roboto', sans-serif;
  text-transform: uppercase;
  outline: 0;
  background: #4CAF50;
  width: 100%;
  border: 0;
  padding: 15px;
  color: #FFFFFF;
  font-size: 14px;
  -webkit-transition: all 0.3 ease;
  transition: all 0.3 ease;
  cursor: pointer;}
.form button:hover,.form button:active,.form button:focus {
  background: #43A047;}
.form .message {
  margin: 15px 0 0;
  color: #b3b3b3;
  font-size: 15px;}
.form .msgshow {
  margin: 0 0 13px;
  color: #b3b3b3;
  font-size: 12px;}
body {
  background: #76b852;
  background: -webkit-linear-gradient(right, #76b852, #8DC26F);
  background: -moz-linear-gradient(right, #76b852, #8DC26F);
  background: -o-linear-gradient(right, #76b852, #8DC26F);
  background: linear-gradient(to left, #76b852, #8DC26F);
  font-family: 'Roboto', sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;}
</style>

<script>
function SaveServerConfig() 
{
  var name = document.getElementById('name').value;
  var ip = document.getElementById('ip').value;
  var port = document.getElementById('port').value;
  if(name == ''){window.alert('请输入设备标识,用于区分多个设备使用!');return false;}
  if(ip == ''){window.alert('请输入收发数据的服务端IP地址!');return false;}
  if(port == ''){window.alert('请输入收发数据的服务端通信端口!');return false;}
  document.getElementById("show").innerHTML = "";
  document.getElementById("save").disabled = true;
  
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange  = function() {
	document.getElementById("save").disabled = false;
    if (this.readyState == 4 && this.status == 200)
	{
		if(this.responseText == "OK"){
			document.getElementById("show").innerHTML = "配置成功,进入配置 WIFI 页面.";
			window.location.href="http://192.168.8.1/WIFI";
		}else{
			document.getElementById("show").innerHTML = "配置失败,保存信息失败,遇到未知错误.";
		}
		window.alert(document.getElementById("show").innerHTML);
    }
  };
  
  xhttp.onerror = function() {
	document.getElementById("show").innerHTML = "保存服务端配置,芯片未响应请求";
	document.getElementById("save").disabled = false;
  };
  
  xhttp.open("GET","/SAVESERVER?name=" + name + "&ip=" + ip + "&port=" + port, true);
  xhttp.send();
  document.getElementById("show").innerHTML = "配置请求已发送, 正在等待配置状态";
}
</script>
</head>
<body>
<div class='login-page'>
    <div class='form'>
	<p id='show' class='msgshow'></p>
	<input id='name' placeholder='设备标识' value='{name}'>
    <input id='ip' placeholder='IP 地址' value='{ip}'>
    <input id='port' placeholder='端口' value='{port}'>
    <button id='save' onclick='javascript:SaveServerConfig();'>保存配置</button>
    <p class='message'>配置透传数据收发服务端通信地址信息</p>
    </div>
</div>
</body>
</html>
)=====";
