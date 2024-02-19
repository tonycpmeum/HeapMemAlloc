build:
	docker build --tag pee:poo .

run:
	docker run -it -v ./app:/app pee:poo
