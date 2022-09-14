# pull the official base image
FROM alpine:3.13
# install packages
RUN apk add --no-cache py3-pip rsync python3 python3-dev make automake gcc g++ linux-headers

# set working direction
WORKDIR /app
# copy files
COPY . /app
# install python requirements
RUN pip3 install -r requirements.txt
# set private key
RUN mkdir /etc/ssh && chmod 0700 /etc/ssh
RUN echo "IdentityFile /app/private-key/chave.pem" >> /etc/ssh/ssh_config
RUN chmod 400 /app/private-key/chave.pem

ENTRYPOINT ["python3"]
CMD ["call_api.py"]
