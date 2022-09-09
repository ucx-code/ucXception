#Base imports
from flask import Flask, request, make_response
import jwt
from flask_mail import Mail, Message
from flask_cors import CORS
from threading import Thread
import datetime, hashlib

#Core
import api.database as database
import api.core.utils as utils
from api.core.abort import abort

#blueprints
from api.blueprints.analisis import analisis
from api.blueprints.callframework import callframework
from api.blueprints.campaign import campaign
from api.blueprints.component import component
from api.blueprints.execution import execution
from api.blueprints.host import host
from api.blueprints.login import login
from api.blueprints.register import register
from api.blueprints.resetpassword import resetpassword
from api.blueprints.tokenverification import tokenverification
from api.blueprints.usercampaign import user_campaign

def create_app():
    app = Flask(__name__)
    #Read env parameters
    app.config.from_pyfile('settings.py')
    #Mail configuration
    app.config['MAIL_USE_TLS'] = True
    app.config['MAIL_USE_SSL'] = False
    app.config['FRAMEWORK_DATA'] = utils.pre_load_data()
    mail = Mail (app)
    
    #Apply CORS
    CORS(app)

    #Load blueprints
    app.register_blueprint(analisis)
    app.register_blueprint(callframework)    
    app.register_blueprint(campaign)
    app.register_blueprint(component)
    app.register_blueprint(execution)
    app.register_blueprint(host)
    app.register_blueprint(login)
    app.register_blueprint(register)
    app.register_blueprint(resetpassword)
    app.register_blueprint(tokenverification)
    app.register_blueprint(user_campaign)


    #Send mail
    def send_async_email(app, msg):
        with app.app_context():
            mail.send(msg)

    def send_email(subject, sender, recipients, text_body):
        msg = Message(subject, sender=sender, recipients=recipients)
        msg.body = text_body
        Thread(target=send_async_email, args=(app, msg)).start()

    #Enpoint create reset token
    @app.route('/reset', methods=['POST'])
    def create_user_password_reset_token():

        url = app.config['FRONT_END_URL'] + '/reset_confirmation/'
        data = request.get_json()

        #Verify if is not none and if contains email
        if data is None or 'email' not in data:
           return abort("Malformed request syntax!", 400)

        user = database.verify_email(data['email'])

        if(user):      
            expiration_time = datetime.datetime.utcnow() + datetime.timedelta(minutes=10)
            reset_token = jwt.encode({'public_id' : user['public_id'], 'exp' : expiration_time}, app.config['SECRET_KEY'], algorithm="HS256")
            encode_token = reset_token.encode('UTF-8')
            hash_object = hashlib.sha224(encode_token)

            if not database.create_reset_password(expiration_time, hash_object.hexdigest(), user['public_id']):
                return abort('Unexpected error while acessing database!', 500)

            body = utils.message_body_constructor(url, reset_token)

            send_email('[ucXception] Reset Your Password',
                                    sender='ucxception@mailtrap.io',
                                    recipients=[user['email']],
                                    text_body=body)
            
            return make_response({"message": "Mail sent!"}, 202, {'Content-Type': 'application/json'})
        else:
            return abort('Mail sent!', 202)

    #Create database if doesnt exist and create component types
    database.database_main()
    
    #Run API
    app.run(host="0.0.0.0", port=app.config['PORT'])