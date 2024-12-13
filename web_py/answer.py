from flask import Flask, request, jsonify
import openai
import os

app = Flask(__name__)

# 这个可以部署自己本地大模型作为回答
# 配置 OpenAI 客户端
openai.api_key = os.getenv("OPENAI_API_KEY", "你的密钥")
openai.api_base = "https://api.chatanywhere.tech/v1"


@app.route('/api/query', methods=['POST'])
def query_openai():
    data = request.json
    if not data or "question" not in data:
        return jsonify({"error": "Invalid request, 'question' is required"}), 400

    question = data["question"]

    try:
        # 使用 openai 客户端进行请求
        completion = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=[
                {"role": "system", "content": "you are a smart assistant!"},
                {"role": "user", "content": question}
            ],
            temperature=0.3
        )

        response_content = completion.choices[0].message.content.strip()
        return jsonify({"response": response_content})
    except openai.error.OpenAIError as e:
        return jsonify({"error": str(e)}), 500


if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5550)
