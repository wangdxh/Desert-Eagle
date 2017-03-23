
<!DOCTYPE html>
<html>
<head>
	
</head>
<body>	
	<script src="static/flv.js"></script>

%if deviceid :
{{deviceid}}<br/>
<video id="videoElement"  width="960" height="540" controls="controls"></video>
<script>
    if (flvjs.isSupported()) {
    	  var urlws = "http://" + window.location.hostname + ":1984" + "/live/liveflv?deviceid={{deviceid}}";
        var videoElement = document.getElementById('videoElement');
        var flvPlayer = flvjs.createPlayer({
            type: 'flv',
            url: urlws
        });
        flvPlayer.attachMediaElement(videoElement);
        flvPlayer.load();
        flvPlayer.play();
    }
</script>
%else:
<h3>Bad Deviceid</h3>
%end
</body>
</html>
