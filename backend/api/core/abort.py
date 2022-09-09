from flask import make_response

def abort(message,code):
    return make_response({"message": message}, code, {'Content-Type': 'application/json'})