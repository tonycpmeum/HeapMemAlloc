build:
	docker build --tag pee:poo .

run:
	docker run -it -v ./app:/app pee:poo

cleanimages:
	docker images -a | grep none | awk '{print $3;}' | xargs docker rmi --force