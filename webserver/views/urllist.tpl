<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title>{{title}}</title>
    </head>
    <body>
    		 %if len(urllist) > 0:
        	%for k in urllist:
        		<a href="/flvplay?deviceid={{k}}"> {{k}} </a><br/>
    			%end        
    		%else:
        	<h3>sorry, no device is online<h3>
    		%end
    		 
    </body>
</html>