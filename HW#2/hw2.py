import requests
import os
from datetime import datetime

def call_api(model, ip):
    data = {
        "model": model,  # Model name
        "messages": chat_history, # Chat history
        "stream": False # Return all messages at once
    }
    
    url = f"http://{ip}:11434/api/chat"
    response = requests.post(url, json=data)
    
    return response


def print_write_file(r_string):
    global f
    
    print(r_string) # Print to console
    
    f = open(f.name, "a")
    f.write(r_string + "\n")
    f.close()


def first_ai(chat_history):
    global first_ai_model, first_ai_ip   # Global variables for first AI model and IP
    
    response = call_api(first_ai_model, first_ai_ip)  # Call the API

    if response.status_code == 200:  # If the response is successful (status code 200)
        
        json_data = response.json()  # Get the JSON data from the response
        
        first_ai_response = json_data['message']  # Get the message from the JSON data
        
        first_ai_response["role"] = "user"  # Set the role to user
        
        chat_history.append(first_ai_response)  # Append the response to the chat history
        
        r_string = f"\nFIRST AI RESPONSE {first_ai_model.upper()} - {first_ai_response['content']}"
                
        print_write_file(r_string)        
        
        second_ai(chat_history)  # Call the second AI
        
    else:
        print(f"Error FIRST AI: {response.status_code}") # Print error message if the response is not successful


def second_ai(chat_history):
    global second_ai_model, second_ai_ip  # Global variables for second AI model and IP
    
    response = call_api(second_ai_model, second_ai_ip)  # Call the API
    
    if response.status_code == 200:   # If the response is successful (status code 200)
        
        json_data = response.json()  # Get the JSON data from the response
        
        second_ai_response = json_data['message']  # Get the message from the JSON data

        second_ai_response["role"] = "user" # Set the role to user
            
        chat_history.append(second_ai_response)  # Append the response to the chat history
        
        r = f"\nSECOND AI RESPONSE {second_ai_model.upper()} - {second_ai_response['content']}"  # Create a string to print and write to file
        r_string = f"\nSECOND AI RESPONSE {second_ai_model.upper()} - {second_ai_response['content']}"
        
        print_write_file(r_string)        

        first_ai(chat_history)
    
    else:
        print(f"SECOND AI Error: {response.status_code}")   # Print error message if the response is not successful
    
    

if __name__ == "__main__":
    chat_history = []
    
    print("Make sure the specified IP has the specified Model installed\n")  # Print a message to the console
    
    first_ai_ip = str(input("First IP: "))   # Get the IP address of the first AI
    first_ai_model = str(input("First Model: "))  # Get the model name of the first AI
    
    second_ai_ip = str(input("Second IP: "))  # Get the IP address of the second AI
    second_ai_model = str(input("Second Model: "))  # Get the model name of the second AI
    
    initial_message = str(input("Initial message: "))  # Get the initial message
    chat_history.append({"role": "user", "content": initial_message})  # Append the initial message to the chat history
    
    d = datetime.now()  # Get the current date and time
    date = datetime.now()
    
    directory = "Conversations"
    
    try:
        os.mkdir(directory)
        print("Directory created")
    except OSError:
        print("Directory already exists")
    
    filename = f"conversation-{date}.txt"
    
    file_path = os.path.join(directory, filename)
    
    f = open(file_path, "x")
    
    first_ai(chat_history)

    
