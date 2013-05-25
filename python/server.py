from http.server import HTTPServer,BaseHTTPRequestHandler  
import io,shutil,urllib   
import inverted,pre
  
class MyHttpHandler(BaseHTTPRequestHandler):  
    def do_GET(self):  
        p=''
        if '?' in self.path:#如果带有参数       
            self.queryString=urllib.parse.unquote(self.path.split('?',1)[1])       
            #name=str(bytes(params['name'][0],'GBK'),'utf-8')       
            params=urllib.parse.parse_qs(self.queryString)       
            p = params['query'][0]
        r_str = open('index.html','r').read()  
        if p!= "":
            search = '<b>SEARCHING FOR '+p+'</b>'+inverted.search(p)
        else:
            search = ''
        result = r_str.replace('{$result}',search)
        enc="UTF-8"  
        encoded = ''.join(result).encode(enc)  
        f = io.BytesIO()  
        f.write(encoded)  
        f.seek(0)  
        self.send_response(200)  
        self.send_header("Content-type", "text/html; charset=%s" % enc)  
        self.send_header("Content-Length", str(len(encoded)))  
        self.end_headers()  
        shutil.copyfileobj(f,self.wfile)  
  
httpd=HTTPServer(('',8080),MyHttpHandler)  
print("Server started on 127.0.0.1,port 8080.....")  
httpd.serve_forever()  