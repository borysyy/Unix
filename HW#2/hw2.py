import requests
from datetime import datetime

def call_api(model, ip):
    data = {
        "model": model,
        "messages": chat_history,
        "stream": False
    }
    
    url = f"http://{ip}:11434/api/chat"
    response = requests.post(url, json=data)
    
    return response


def print_write_file(r):
    global f
    
    print(r)
    
    f = open(f.name, "a")
    f.write(r)
    f.close()


def first_ai(chat_history):
    global first_ai_model, first_ai_ip
    
    response = call_api(first_ai_model, first_ai_ip)

    if response.status_code == 200:
        
        json_data = response.json()
        
        first_ai_response = json_data['message']
        
        first_ai_response["role"] = "user"
        
        chat_history.append(first_ai_response)
        
        r = f"\nFIRST AI RESPONSE {first_ai_model.upper} - {first_ai_response['content']}"
                
        print_write_file(r)        
        
        second_ai(chat_history)
        
    else:
        print(f"Error FIRST AI: {response.status_code}")


def second_ai(chat_history):
    global second_ai_model, second_ai_ip
    
    response = call_api(second_ai_model, second_ai_ip)
    
    if response.status_code == 200:
        
        json_data = response.json()
        
        second_ai_response = json_data['message']

        second_ai_response["role"] = "user"
            
        chat_history.append(second_ai_response)
        
        r = f"\nSECOND AI RESPONSE {second_ai_model.upper} - {second_ai_response['content']}"
        
        print_write_file(r)        

        first_ai(chat_history)
    
    else:
        print(f"SECOND AI Error: {response.status_code}")
    
    

if __name__ == "__main__":
    chat_history = []
    
    print("Make sure the specified IP has the specified Model installed\n")
    
    first_ai_ip = str(input("First IP: "))
    first_ai_model = str(input("First Model: "))
    
    second_ai_ip = str(input("Second IP: "))
    second_ai_model = str(input("Second Model: "))
    
    initial_message = str(input("Initial message: "))
    chat_history.append({"role": "user", "content": initial_message})
    
    d = datetime.now()
    
    f = open(f"conversation-{d}.txt", "x")
    
    first_ai(chat_history)

    
