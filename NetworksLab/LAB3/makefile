all:receiver sender

receiver: receiver.c
	gcc -o receiver -pthread receiver.c -lm

sender: sender.c
	gcc -o sender -pthread sender.c -lm
	
clean:
	rm receiver sender