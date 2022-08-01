from flask import Blueprint, request, make_response, current_app
import jwt
from api.core.abort import abort

tokenverification = Blueprint("tokenverification", __name__)

#Verify if token is still valid
@tokenverification.route('/verify_login_token', methods=['POST'])
def verify_token():
    token = None

    if 'Authentication' in request.headers:
        token = request.headers['Authentication']

    if not token:
        return abort("MToken is missing!", 401)

    try:
        jwt.decode(token, current_app.config['SECRET_KEY'], algorithms=["HS256"])
    except jwt.ExpiredSignatureError:
        return abort("Token expired. Get new one!", 401)
    except jwt.InvalidTokenError:
        return abort("Invalid Token!", 401)
    
    return make_response({'message' : 'Token is verified!'}, 200, {'Content-Type': 'application/json'})