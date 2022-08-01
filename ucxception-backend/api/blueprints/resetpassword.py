from flask import Blueprint, request, make_response, current_app
from werkzeug.security import generate_password_hash
import jwt
from api.core.abort import abort
import api.database as database
import api.core.utils as utils
import hashlib

resetpassword = Blueprint("resetpassword", __name__)

#CHANGE PASSWORD WITH TOKEN
@resetpassword.route('/reset/<reset_token>', methods=['PUT'])
def change_user_password(reset_token):

    data = request.get_json()

    #Verify if is not none and if contains email and password
    if data is None or 'password' not in data or 'repassword' not in data:
        return abort("Malformed request syntax!", 400)

    if reset_token:
        try:
            jwt.decode(reset_token, current_app.config['SECRET_KEY'], algorithms=["HS256"])
            #Verify if token is correct
            hash_token = hashlib.sha224(reset_token.encode('utf-8'))
            reset_password_userid = database.verify_reset_token(hash_token.hexdigest())

            #If correct
            if reset_password_userid:
                #Verify passwords
                if data['password'] == data['repassword']:
                    if not utils.validate_password(data['password']):
                        return abort("Minimum eight characters, at least one uppercase letter, one lowercase letter and one number.", 422)
                else:
                    return abort("Passwords do not match!", 403)

                hashed_password = generate_password_hash(data['password'], method='sha256')
                #Password changed
                if database.change_password(reset_password_userid['user_iduser'], hashed_password):
                    return make_response({"message": "Password changed successfully!"}, 200, {'Content-Type': 'application/json'})
                else:
                    return abort('Password not changed!', 500)

            #Token couldnt be validate
            else:
                return abort('Token is not valid!', 401)

        except jwt.ExpiredSignatureError:
            return abort('Token expired.', 401)
        except jwt.InvalidTokenError:
            return abort('Invalid token!', 401)
    else:
        return abort('Token is missing!', 401)

#REQUEST RESET TOKEN
@resetpassword.route('/verify_reset_token/<reset_token>', methods=['POST'])
def verify_user_password_reset_token(reset_token):

    if reset_token:
        try:
            jwt.decode(reset_token, current_app.config['SECRET_KEY'], algorithms=["HS256"])
            #Verify if token is correct
            hash_token = hashlib.sha224(reset_token.encode('utf-8'))
            reset_password = database.verify_reset_token(hash_token.hexdigest())

            #If correct
            if reset_password:
                return make_response({'message' : 'Valid token!'}, 200, {'Content-Type': 'application/json'})
            else:
                return abort('Token is not valid!', 401)

        except jwt.ExpiredSignatureError:
            return abort('Token expired.', 401)
        except jwt.InvalidTokenError:
            return abort('Invalid Token!', 401)
    else:
        return abort('Token is missing!', 401)
