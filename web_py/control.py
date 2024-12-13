from flask import Flask, request
import pyautogui

app = Flask(__name__)

@app.route('/flip_next', methods=['POST'])
def flip_next():
    pyautogui.press("right")  # 下一页
    return "Next Slide", 200

@app.route('/flip_prev', methods=['POST'])
def flip_prev():
    pyautogui.press("left")  # 上一页
    return "Previous Slide", 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5100)