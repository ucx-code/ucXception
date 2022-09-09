from flask import Blueprint, request, make_response, current_app
from werkzeug.security import check_password_hash
import jwt
from api.core.abort import abort
import api.database as database
import api.core.utils as utils
import datetime

login = Blueprint("login", __name__)

@login.route('/login', methods=['POST'])
def login_function():

    #Read from body
    auth = request.get_json()

    #Verify if is not none and if contains email and password
    if auth is None or 'email' not in auth or 'password' not in auth:
        return abort("Malformed request syntax!", 400)

    #Verify email
    if not utils.validate_email(auth['email']):
        return abort("Email is not valid!", 400)

    #Verify password
    if not utils.validate_password(auth['password']):
        return abort("Minimum eight characters, at least one uppercase letter, one lowercase letter and one number.", 422)

    #Get user from database
    user = database.get_user_from_email(auth["email"])

    #If user does not exist
    if not user:
        return abort('Invalid email or password!', 422)

    #Verify passwords
    if check_password_hash(user["password"], auth["password"]):
        #Generate token
        token = jwt.encode({'public_id' : user['public_id'], 'exp' : datetime.datetime.utcnow() + datetime.timedelta(minutes=100)}, current_app.config['SECRET_KEY'], algorithm="HS256")
        #Retrieve toke
        retrive_json = {'token' : token,
                'user': {
                            'public-id': user['public_id'],
                            'email': user['email'],
                            'username': user['username']
                        }
                }
        return make_response({"user": retrive_json ,"message": "User logged in!"}, 200, {'Content-Type': 'application/json'})
    else:
        return abort('Invalid email or password!', 422)