#!/usr/bin/env python3

# http://localhost:65531/get_key_statistics?i=key&v=v3
# nohup python3 ../ylog/index.py -l -p 65531 >> log.log 2>&1 &

import flask
import ylog_for_python
import sys

class YGlobal(object):
    flask_app = flask.Flask(__name__)
    args = None
    # level(0:debug, 1:info, 2:err), file, type(0:add, 1:over)
    log = ylog_for_python.YLog(0, "yout.log", 0)

@YGlobal.flask_app.route('/get_key_statistics',methods=['GET'])
def Test():
    ra_info = flask.request.args.get('i')
    if ra_info == None:
        return 'info error'
    ra_value = flask.request.args.get('v')
    if ra_value == None:
        return 'value error'
    YGlobal.log.W(__file__, sys._getframe().f_lineno, 1, ra_info, ra_value)
    return 'ok, {}:{}'.format(ra_info, ra_value)

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--port', default='80', action='store',
        help='set the webapp\'s port')
    parser.add_argument('-l', '--localhost', action="store_true",
        help='use localhost')
    YGlobal.args = parser.parse_args()
    YGlobal.flask_app.config.update({'TEMPLATES_AUTO_RELOAD': True})
    YGlobal.flask_app.config.update({'SECRET_KEY': 'fajweofajjjgljxz'})
    host = None if YGlobal.args.localhost else '0.0.0.0'
    YGlobal.flask_app.run(port=int(YGlobal.args.port), host=host)

