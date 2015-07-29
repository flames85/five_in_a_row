CC 			= g++
CLIENT_APP 	= client
SERVER_APP 	= server
PIECE_OBJ 	= piece.o
FLAG 		= -g -Wall -DCUR_MODE
RM 			= rm -f

# final target
all: $(CLIENT_APP) $(SERVER_APP) 

# client app
$(CLIENT_APP) : $(PIECE_OBJ) client.cpp
	$(CC) $(FLAG) $^ -o $@

# server app
$(SERVER_APP) : $(PIECE_OBJ) server.cpp 
	$(CC) $(FLAG) $^ -o $@

# piece logic
$(PIECE_OBJ) : piece.cpp piece.h
	$(CC) $(FLAG) -c piece.cpp -o $@ 

# clean all
clean:
	$(RM) $(PIECE_OBJ) $(CLIENT_APP) $(SERVER_APP)
