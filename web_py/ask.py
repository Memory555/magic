from flask import Flask, request, render_template_string, jsonify, redirect, url_for
import requests

# 不一定要在电脑提交问题，在同一个局域网下（同一热点）的手机下都可以，只需要在网页中输入http://192.168.224.100:5000/api/ask
app = Flask(__name__)

# ESP8266 的 IP 地址（需要替换为实际的 ESP8266 IP）
ESP8266_IP = 'http://192.168.224.251/'

@app.route('/api/ask', methods=['GET', 'POST'])
def ask():
    # 渲染一个美观的 HTML 表单
    html = '''
        <!doctype html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>问问题</title>
            <style>
                body {
                    font-family: 'Arial', sans-serif;
                    background-color: #f7f7f7;
                    display: flex;
                    justify-content: center;
                    align-items: center;
                    height: 100vh;
                    margin: 0;
                }
                .container {
                    background-color: #ffffff;
                    border-radius: 8px;
                    box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
                    padding: 20px;
                    width: 100%;
                    max-width: 400px;
                    text-align: center;
                }
                h1 {
                    color: #333;
                    margin-bottom: 20px;
                }
                input[type="text"] {
                    width: calc(100% - 20px);
                    padding: 10px;
                    border: 1px solid #ddd;
                    border-radius: 4px;
                    margin-bottom: 10px;
                    font-size: 16px;
                }
                input[type="submit"] {
                    background-color: #28a745;
                    color: white;
                    border: none;
                    padding: 10px 20px;
                    border-radius: 4px;
                    cursor: pointer;
                    font-size: 16px;
                    transition: background-color 0.3s;
                }
                input[type="submit"]:hover {
                    background-color: #218838;
                }
                .message {
                    margin-top: 20px;
                    font-size: 14px;
                    color: #555;
                }
            </style>
            <script>
                function displayMessage() {
                    alert('问题已提交，谢谢！');
                }
            </script>
        </head>
        <body>
            <div class="container">
                <h1>输入你的问题</h1>
                <form method="post" onsubmit="displayMessage()">
                    <input type="text" name="question" placeholder="输入问题" required>
                    <br>
                    <input type="submit" value="提交">
                </form>
                <div class="message">
                    请在输入框中输入问题，然后点击提交按钮。
                </div>
            </div>
        </body>
        </html>
        '''

    if request.method == 'POST':
        # 获取用户提交的问题
        question = request.form.get('question')
        if question:
            # 发送请求到 ESP8266
            response = requests.post(ESP8266_IP + 'question', data={'question': question})
            if response.status_code == 200:
                # 提交成功后重新加载页面并清空输入框
                return render_template_string(html, message="问题已提交！")
            else:
                return render_template_string(html, message="发送问题失败，请重试。")
        #
        # else:
        #     return jsonify({'status': 'error', 'message': 'No question provided'}), 400

    return render_template_string(html)

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
