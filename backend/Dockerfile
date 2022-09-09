# pull the official base image
FROM ubuntu:18.04
# install packages
RUN apt-get update && \
    apt-get install -y python3.8 python3-pip python3.8-dev ssh rsync
# set working direction
WORKDIR /app
# copy files
COPY . /app
# install python requirements
RUN pip3 install -r requirements.txt
# set private key
RUN echo "IdentityFile /app/private_key/chave.pem" >> /etc/ssh/ssh_config
RUN chmod 400 /app/private_key/chave.pem

ENTRYPOINT ["python3"]
CMD ["call_api.py"]