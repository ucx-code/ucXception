import jwt
from flask import request, make_response, current_app
from functools import wraps
import api.database as database

#ERROR MESSAGE
def abort(message,code):
    return make_response({"message": message}, code, {'Content-Type': 'application/json'})

def token_required(f):
    @wraps(f)
    def decorated(*args, **kwargs):
        token = None
        current_user = {}

        if 'Authentication' in request.headers:
            token = request.headers['Authentication']

        if not token:
            return abort('Token is missing!', 401)
        
        try:
            data = jwt.decode(token, current_app.config['SECRET_KEY'], algorithms=["HS256"])
            data_current_user = database.get_user_from_public_id(data['public_id'])
            if data_current_user:
                current_user = {'id': data_current_user['iduser'], 'public_id': data_current_user['public_id'], 'email': data_current_user["email"], 'username': data_current_user['username']}
            else: 
                return abort('User public id not valid!', 401)
        except jwt.ExpiredSignatureError:
            return abort('Token expired. Get new one!', 401)
        except jwt.InvalidTokenError:
            return abort('Invalid Token!', 401)

        return f(current_user, *args, **kwargs)

    return decorated