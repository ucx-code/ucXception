#!/bin/bash

# garantir que os seguintes pacotes estão instalados
#	gcc				# para compilar
#	wget			# para fazer download dos pacotes
#	make			# para compilar e instalar
#	w3m 			# para testar o server via linha de comandos
#	shellcheck 		# para verificar o script (spellcheck myscript)
#	libpcre3-dev	# Perl Compatible Regular Expressions
sudo apt-get install -y gcc wget make w3m shellcheck libpcre3-dev

mkdir apache2
cd apache2

# sacar os pacotes
wget http://mirrors.fe.up.pt/pub/apache//httpd/httpd-2.4.17.tar.gz
wget http://mirrors.fe.up.pt/pub/apache//apr/apr-1.5.2.tar.gz
wget http://mirrors.fe.up.pt/pub/apache//apr/apr-util-1.5.4.tar.gz
wget ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/pcre2-10.20.tar.gz

wget http://www.php.net/distributions/php-5.6.14.tar.xz

# extrair os pacotes
tar -xzvf httpd-2.4.17.tar.gz
tar -xzvf apr-1.5.2.tar.gz
tar -xzvf apr-util-1.5.4.tar.gz
tar -xzvf pcre2-10.20.tar.gz
tar xf php-5.6.14.tar.x
z

# move-los para a sourcelib do apache
mv apr-1.5.2 httpd-2.4.17/srclib/apr
mv apr-util-1.5.4 httpd-2.4.17/srclib/apr-util
mv pcre2-10.20 httpd-2.4.17/srclib/pcre

# biblioteca necessária para meter o php a funcionar
sudo apt-get install -y libxml1-dev


# selecionar o ficheiro e gerar modificações
# para cada um dos patchs criados, configurar e instalar o apache2 e o php
# criar pagina php nos sites a fazer deploy


# configurar e instalar o PHP
cd php-5.6.14
./configure

# configurar e instalar o Perl Compatible Regular Expressions
cd httpd-2.4.17/srclib/pcre
./configure --prefix=/usr/local/pcre
make
make install

# configurar o apache2 ou httpd
cd ../..
./configure --with-included-apr --with-pcre=/usr/local/pcre
# make -j 2		# criar várias threads
make
make install

./httpd
w3m localhost # testar o apache