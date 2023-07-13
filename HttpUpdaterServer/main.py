from flask import Flask, request, send_from_directory
import os

SERVER_HOST = "192.168.100.117"
SERVER_PORT = 3000
FIRMWARE_DIRECTORY = "firmwares"

api = Flask(__name__)
api.config['FIRMWARE_DIRECTORY'] = FIRMWARE_DIRECTORY

@api.route('/update/<firmware_name>', methods=['GET', 'POST'])
def download_firmware(firmware_name):
  firmwares = os.listdir(api.config['FIRMWARE_DIRECTORY'])
  for firmware in firmwares:
    _version_part = firmware.split("_")[-1]
    _firmware_name = firmware.removesuffix(_version_part).removesuffix("_")

    if _firmware_name == firmware_name:
      latest_version = _version_part.removesuffix(".bin")
      if "x-ESP32-version" not in request.headers:
        return "Error: Missing current version", 400

      if request.headers["x-ESP32-version"] != latest_version:
        return send_from_directory(directory=api.config['FIRMWARE_DIRECTORY'], path=firmware,
                                   as_attachment=True, mimetype='application/octet-stream')
      else:
        return "No update available", 304

  return "Error: Unknown firmware", 400

if __name__ == '__main__':
    api.run(host=SERVER_HOST, port=SERVER_PORT)
