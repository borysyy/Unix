# CS 528 - Unix Network Programming
# Program: client.c
# Assignment: HW#2
# Authors: Slava Borysyuk, Taylor Hunter
# Date: 02/09/2025
# This program is a client that connects to two different AI models and sends messages to 
# them in a loop. The client will send a message to the first AI model, then send the response 
# to the second AI model, and then send the response back to the first AI model. 
# The conversation will continue until the user stops the program. The client will write the 
# conversation to a file with the current date and time in the filename.

import requests  # Import the requests module
from datetime import datetime  # Import the datetime module

# Define the API endpoint URL
def call_api(model, ip):
    data = {
        "model": model,  # Model name
        "messages": chat_history, # Chat history
        "stream": False # Return all messages at once
    }
    
    url = f"http://{ip}:11434/api/chat"
    response = requests.post(url, json=data)
    
    return response

# Print and write to file
def print_write_file(r):
    global f  # File object
    
    print(r) # Print to console
    
    f = open(f.name, "a")  # Open file in append mode
    f.write(r)  # Write to file
    f.close()  # Close file

# Main function to call the first AI
def first_ai(chat_history):
    global first_ai_model, first_ai_ip   # Global variables for first AI model and IP
    
    response = call_api(first_ai_model, first_ai_ip)  # Call the API

    if response.status_code == 200:  # If the response is successful (status code 200)
        
        json_data = response.json()  # Get the JSON data from the response
        
        first_ai_response = json_data['message']  # Get the message from the JSON data
        
        first_ai_response["role"] = "user"  # Set the role to user
        
        chat_history.append(first_ai_response)  # Append the response to the chat history
        
        r = f"\nFIRST AI RESPONSE {first_ai_model.upper()} - {first_ai_response['content']}" # Create a string to print and write to file
                
        print_write_file(r)     # Print and write to file
        
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
        
        print_write_file(r)      # Print and write to file

        first_ai(chat_history)  # Call the first AI again
    
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
    
    f = open(f"conversation-{d}.txt", "x")  # Create a new file with the current date and time
    
    first_ai(chat_history)  # Call the first AI

    
