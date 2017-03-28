#!/usr/bin/python
#coding: utf-8
# by little_wang
import os
from bottle import route, run, request, response, static_file,get, post, view
import urllib2
import json

g_maplist = dict()
g_maplist['123abcdef32153421'] = '123abcdef32153421'

print 'Moriturus te saluto!!!'


images_path = './static/'
@route('/static/<filename>')
def server_static(filename):    
    return static_file(filename, root=images_path)

@get('/')
@get('/index.html')
@view('urllist')
def index():
    # get urllist from streamserver
    '''headers = {'Accept':'application/json'}
    req = urllib2.Request('http://www.baidu.com', headers=headers)    
    response = urllib2.urlopen(req)  
    hjson = json.loads(response.read())
    maplist = hjson.get('deviceidlist', None)
    print hjson'''
    
    return {'urllist':g_maplist, 'title':'little_wang'}

@get('/flvplay')
@view('flvplay')
def liveflv():
    deviceid = request.params.get('deviceid', None)
    if  deviceid != None:
        return {'deviceid':deviceid}
    else:        
        return {'deviceid':None}
if __name__ == '__main__':
    run(host='0.0.0.0', port=80)
