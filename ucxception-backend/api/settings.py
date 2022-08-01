from os import environ

SECRET_KEY = environ.get('SECRET_KEY')
FRONT_END_URL = environ.get('FRONT_END_URL')
PORT = environ.get('PORT')

MAIL_SERVER= environ.get('MAIL_SERVER')
MAIL_PORT= environ.get('MAIL_PORT')
MAIL_USERNAME= environ.get('MAIL_USERNAME')
MAIL_PASSWORD= environ.get('MAIL_PASSWORD')