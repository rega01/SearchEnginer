all:searchEngine bin/precessorPage bin/DictProducer 

searchEngine:bin/main.o  bin/InetAddr.o bin/Acceptor.o \
	bin/TcpServer.o bin/TcpConnection.o bin/TaskQueue.o bin/Task.o\
	bin/Socket.o bin/SockIo.o bin/ThreadPool.o bin/EventLoop.o bin/SearchServer.o\
	bin/KeyRecommander.o bin/Dictionary.o bin/Configuration.o bin/WebPageSearcher.o\
	bin/WebPageQuery.o bin/SplitToolCppJieba.o bin/LRU.o bin/RedisServer.o
	g++ bin/main.o bin/SearchServer.o bin/InetAddr.o bin/Acceptor.o bin/TcpServer.o\
		bin/TcpConnection.o bin/TaskQueue.o bin/Task.o bin/Socket.o bin/SockIo.o \
		bin/ThreadPool.o bin/EventLoop.o bin/WebPageSearcher.o bin/RedisServer.o\
		bin/KeyRecommander.o bin/Dictionary.o bin/Configuration.o bin/WebPageQuery.o\
		bin/SplitToolCppJieba.o bin/LRU.o -o ./SearchServer -lpthread -lhiredis -lxapian

bin/precessorPage:src/precessorPage.cc bin/PageLibPreprocessor.o  bin/DirScanner.o bin/SplitToolCppJieba.o bin/Configuration.o
	g++ src/precessorPage.cc bin/PageLibPreprocessor.o bin/DirScanner.o bin/SplitToolCppJieba.o bin/Configuration.o -o bin/precessorPage  -I ./include -g -Wall -ltinyxml2 -lpthread -lxapian

bin/DictProducer:bin/Dict.o bin/DictProducer.o bin/DirScanner.o \
	bin/SplitToolCppJieba.o bin/Configuration.o 
	g++ bin/Dict.o bin/DictProducer.o bin/DirScanner.o bin/SplitToolCppJieba.o\
		bin/Configuration.o -o bin/DictProducer -lpthread

bin/main.o:src/main.cc
	g++  -c src/main.cc -o bin/main.o -I ./include -g -Wall

bin/LRU.o:src/LRU.cc include/LRU.h
	g++  -c src/LRU.cc -o bin/LRU.o -I ./include -g -Wall

bin/DirScanner.o:src/DirScanner.cc include/DirScanner.h
	g++ -c src/DirScanner.cc -o bin/DirScanner.o -I ./include -g -Wall

bin/SplitToolCppJieba.o:src/SplitToolCppJieba.cc include/SplitToolCppJieba.h\
	include/Configuration.h
	g++ -c src/SplitToolCppJieba.cc -o bin/SplitToolCppJieba.o -I ./include -g -Wall

bin/DictProducer.o:src/DictProducer.cc include/DictProducer.h
	g++ -c src/DictProducer.cc -o bin/DictProducer.o -I ./include -g -Wall

bin/PageLibPreprocessor.o:src/PageLibPreprocessor.cc include/PageLibPreprocessor.h \
	include/DirScanner.h include/SplitToolCppJieba.h
	g++ -c src/PageLibPreprocessor.cc -o bin/PageLibPreprocessor.o -I ./include -g -Wall

bin/Configuration.o:src/Configuration.cc include/Configuration.h
	g++ -c src/Configuration.cc -o bin/Configuration.o -I ./include -g -Wall

bin/Dict.o:src/Dict.cc 
	g++ -c src/Dict.cc -o bin/Dict.o -I ./include -g -Wall

bin/InetAddr.o:src/InetAddr.cc include/InetAddr.h
	g++ -c src/InetAddr.cc -o bin/InetAddr.o -I ./include -g -Wall

bin/Acceptor.o:src/Acceptor.cc include/Acceptor.h
	g++ -c src/Acceptor.cc -o bin/Acceptor.o -I ./include -g -Wall

bin/TcpServer.o:src/TcpServer.cc include/TcpServer.h
	g++ -c src/TcpServer.cc -o bin/TcpServer.o -I ./include -g -Wall
		
bin/TcpConnection.o:src/TcpConnection.cc include/TcpConnection.h
	g++ -c src/TcpConnection.cc -o bin/TcpConnection.o -I ./include -g -Wall

bin/TaskQueue.o:src/TaskQueue.cc include/TaskQueue.h
	g++ -c src/TaskQueue.cc -o bin/TaskQueue.o -I ./include -g -Wall

bin/Task.o:src/Task.cc include/Task.h
	g++ -c  src/Task.cc -o bin/Task.o -I ./include -g -Wall

bin/Socket.o:src/Socket.cc include/Socket.h
	g++ -c src/Socket.cc -o bin/Socket.o -I ./include -g -Wall

bin/SockIo.o:src/SockIo.cc include/SockIo.h
	g++ -c src/SockIo.cc -o bin/SockIo.o -I ./include -g -Wall

bin/ThreadPool.o:src/ThreadPool.cc include/ThreadPool.h
	g++ -c src/ThreadPool.cc -o bin/ThreadPool.o -I ./include -g -Wall

bin/EventLoop.o:src/EventLoop.cc include/EventLoop.h
	g++ -c src/EventLoop.cc -o bin/EventLoop.o -I ./include -g -Wall

bin/SearchServer.o:src/SearchServer.cc include/SearchServer.h
	g++ -c src/SearchServer.cc -o bin/SearchServer.o -I ./include -g -Wall

bin/Dictionary.o:src/Dictionary.cc include/Dictionary.h
	g++ -c src/Dictionary.cc -o bin/Dictionary.o -I ./include -g -Wall

bin/KeyRecommander.o:src/KeyRecommander.cc include/KeyRecommander.h
	g++ -c src/KeyRecommander.cc -o bin/KeyRecommander.o -I ./include -g -Wall

bin/WebPageQuery.o:src/WebPageQuery.cc include/WebPageQuery.h
	g++ -c src/WebPageQuery.cc -o bin/WebPageQuery.o -I ./include -g -Wall

bin/WebPageSearcher.o:src/WebPageSearcher.cc include/WebPageSearcher.h
	g++ -c src/WebPageSearcher.cc -o bin/WebPageSearcher.o -I ./include -g -Wall

bin/RedisServer.o: src/RedisServer.cc include/RedisServer.h
	g++ -c src/RedisServer.cc -o bin/RedisServer.o -I ./include -g -Wall 

test:bin/testdir bin/testSplit bin/testConf bin/testReactor bin/client\
	 # bin/testWebPageSearcher  

bin/testWebPageSearcher:bin/testWebPageSearcher.o bin/WebPageQuery.o\
	bin/WebPageSearcher.o bin/Configuration.o bin/SplitToolCppJieba.o\
	bin/TcpConnection.o
	g++ bin/testWebPageSearcher.o bin/WebPageQuery.o bin/WebPageSearcher.o\
		bin/Configuration.o bin/SplitToolCppJieba.o bin/TcpConnection.o\
	bin/RedisServer.o	-o bin/testWebPageSearcher -lpthread

bin/testWebPageSearcher.o:src/testWebPageSearcher.cc 
	g++ -c src/testWebPageSearcher.cc  -o bin/testWebPageSearcher.o -I ./include -g -Wall

bin/client:src/testClient.cc
	g++ src/testClient.cc -o bin/client -I ./include -g -Wall

bin/testKeyRecommander:bin/testKeyRecommander.o bin/KeyRecommander.o\
				bin/Dictionary.o bin/Configuration.o bin/TcpConnection.o\
	bin/TcpServer.o bin/TcpConnection.o bin/TaskQueue.o bin/Task.o\
	bin/Socket.o bin/SockIo.o bin/ThreadPool.o bin/EventLoop.o bin/SearchServer.o\
	bin/WebPageSearcher.o 
	g++ bin/testKeyRecommander.o bin/KeyRecommander.o bin/Dictionary.o\
		 bin/InetAddr.o bin/Acceptor.o bin/TcpServer.o\
		bin/TcpConnection.o bin/TaskQueue.o bin/Task.o bin/Socket.o bin/SockIo.o \
		bin/ThreadPool.o bin/EventLoop.o bin/SearchServer.o -o bin/testReactor\
		bin/WebPageSearcher.o \
		bin/Configuration.o  -o bin/testKeyRecommander -lpthread

bin/testKeyRecommander.o:src/testKeyRecommander.cc 
	g++ -c src/testKeyRecommander.cc  -o bin/testKeyRecommander.o -I ./include -g -Wall

bin/testReactor:bin/testReactor.o  bin/InetAddr.o bin/Acceptor.o \
	bin/TcpServer.o bin/TcpConnection.o bin/TaskQueue.o bin/Task.o \
	bin/Socket.o bin/SockIo.o bin/ThreadPool.o bin/EventLoop.o bin/SearchServer.o \
	bin/KeyRecommander.o bin/Dictionary.o bin/Configuration.o bin/WebPageSearcher.o \
	bin/WebPageQuery.o bin/SplitToolCppJieba.o bin/LRU.o
	g++ bin/testReactor.o  bin/InetAddr.o bin/Acceptor.o bin/TcpServer.o\
		bin/TcpConnection.o bin/TaskQueue.o bin/Task.o bin/Socket.o bin/SockIo.o \
		bin/ThreadPool.o bin/EventLoop.o bin/SearchServer.o bin/WebPageSearcher.o\
		bin/KeyRecommander.o bin/Dictionary.o bin/Configuration.o bin/WebPageQuery.o\
		bin/RedisServer.o\
		bin/SplitToolCppJieba.o bin/LRU.o -o bin/testReactor -lpthread -lhiredis -lxapian

bin/testReactor.o:src/testReactor.cc
	g++ -c src/testReactor.cc -o bin/testReactor.o -I ./include -g -Wall

bin/testdir:bin/testdir.o bin/DirScanner.o
	g++ bin/testdir.o bin/DirScanner.o -o bin/testdir

bin/testdir.o: src/testdir.cc
	g++ -c src/testdir.cc -o bin/testdir.o -I ./include -g -Wall

bin/testSplit:bin/testSplit.o bin/SplitToolCppJieba.o bin/Configuration.o
	g++ bin/testSplit.o bin/SplitToolCppJieba.o bin/Configuration.o -o bin/testSplit -lpthread

bin/testSplit.o:src/testSplit.cc
	g++ -c src/testSplit.cc -o bin/testSplit.o -I ./include -g -Wall


# bin/precessorPage.o:src/precessorPage.cc
# 	g++ -c src/precessorPage.cc -o bin/precessorPage.o -I ./include -g -Wall
bin/testConf:src/testConf.cc bin/Configuration.o
	g++ src/testConf.cc bin/Configuration.o -o bin/testConf -I ./include -g -Wall -lpthread
