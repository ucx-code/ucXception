from flask import Blueprint, request, make_response
from werkzeug.security import generate_password_hash
from api.core.abort import abort
import api.database as database
import api.core.utils as utils
import uuid

register = Blueprint("register", __name__)

@register.route('/register', methods=['POST'])
def register_function():
    data = request.get_json()
    
    #Verify if is not none and if contains necessary parameters
    if data is None or 'email' not in data or 'username' not in data or 'password' not in data or 'repassword' not in data:
        return abort("Malformed request syntax!", 400)

    #Verify email
    if not utils.validate_email(data['email']):
        return abort("Email is not valid!", 400)

    #Verify if contain strings
    if not isinstance(data['username'], str) or not isinstance(data['password'], str) or not isinstance(data['repassword'], str):
        return abort("Only strings are accepted!", 400)

    if data['password'] == data['repassword']:
        if not utils.validate_password(data['password']):
            return abort("Minimum eight characters, at least one uppercase letter, one lowercase letter and one number.", 422)
    else:
        return abort("Passwords do not match!", 403)

    #Verify if user already registed
    if database.get_user_from_email(data['email']):
        return abort("Existing user with email!", 409)

    hashed_password = generate_password_hash(data['password'], method='sha256')
    if database.create_user(str(uuid.uuid4()), data['username'], data['email'], hashed_password):
        return make_response({'message': 'User created successfully!'}, 201, {'Content-Type': 'application/json'})
    else:
        return abort("Fail to create user!", 422)
