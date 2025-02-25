from ollama import Client

client = Client(
    host="http://localhost:11435",
    headers={
        'Content-Type': 'application/json'
    }
)

chat_history = []

while(1):
    user_input = str(input("Message: "))
    chat_history.append({"role": "user", "content": user_input})

    response = client.chat(
        model="llama3.1",
        messages=chat_history
    )

    chat_history.append(response['message'])
    ai_response = response['message']['content']
    
    print(f"AI Response {ai_response}")

