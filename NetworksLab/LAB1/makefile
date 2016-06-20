all : server client
server : ./FTP-SERVER/ftpserver.c
	        gcc ./FTP-SERVER/ftpserver.c -o ./FTP-SERVER/ftpserver
client : ftpclient.c
	        gcc ftpclient.c -o ftpclient
clean:
	rm ftpclient ./FTP-SERVER/ftpserver	
