import requests
import json
from pprint import pprint

def call_api(input):
    data = {
            "model": "llama3.1",
            "messages": [{"role":"user", "content": input}],
            "stream": False
        }
    url = "http://localhost:11434/api/chat"
    response = requests.post(url, json=data)
    
    if response.status_code == 200:
        # pprint(response.json())
        json_data = response.json()
        ai_response = json_data['message']['content']
    else:
        print(f"Error: {response.status_code}")
    
    return ai_response


if __name__ == "__main__":
    
    # Keep getting a question from the user
    while(1):
        
        question = str(input("Question: "))
        
        if question == "exit":
            break
        
        response = call_api(question)
       
        print(f"AI RESPONSE: {response}")
        
 
        
        
                
