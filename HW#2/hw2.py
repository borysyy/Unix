# CS 528 - Unix Network Programming
# Program: hw2.py
# Assignment: HW#2
# Authors: Slava Borysyuk, Taylor Hunter
# Date: 03/02/2025
# This program is a client that connects to two different AI models and sends messages to 
# them in a loop. The client will send a message to the first AI model, then send the response 
# to the second AI model, and then send the response back to the first AI model. 
# The conversation will continue until the user stops the program. The client will write the 
# conversation to a file with the current date and time in the filename.

import requests # Import the requests module
import os  # Import the os module
from datetime import datetime # Import the datetime module

# Function to call the API of the first AI model and the second AI model
def call_api(model, chat_history, ip):
    # Data to send to the API
    data = {   
        "model": model,  # Model name 
        "messages": chat_history, # Chat history 
        "stream": False # Return all messages at once 
    }
    
    url = f"http://{ip}:11434/api/chat"        # URL to send the data to the API
    response = requests.post(url, json=data)   # Send the data to the API and get the response
    
    return response # Return the response

# Function to write the conversation to a file with the current date and time in the filename
# and print the conversation to the console
def print_write_file(r_string):
    global file_path  # Global variable for the file path
    
    print(r_string) # Print to console
    
    f = open(file_path, "a")  # Open the file in append mode
    f.write(r_string + "\n")  # Write to the file
    f.close()  # Close the file

# Function to call the first AI model
def first_ai(chat_history):
    global first_ai_model, first_ai_ip   # Global variables for first AI model and IP
    
    response = call_api(first_ai_model, chat_history, first_ai_ip)  # Call the API

    if response.status_code == 200:  # If the response is successful (status code 200)
        
        json_data = response.json()  # Get the JSON data from the response
        
        first_ai_response = json_data['message']  # Get the message from the JSON data
        
        first_ai_response["role"] = "user"  # Set the role to user
        
        chat_history.append(first_ai_response)  # Append the response to the chat history
        
        r_string = f"\nFIRST AI RESPONSE {first_ai_model.upper()} - {first_ai_response['content']}" # Create a string to print and write to file
                
        print_write_file(r_string)      # Print and write to file
        
        second_ai(chat_history)  # Call the second AI
        
    else:
        print(f"FIRST AI Error: {response.status_code}") # Print error message if the response is not successful

# Function to call the second AI model
def second_ai(chat_history):
    global second_ai_model, second_ai_ip  # Global variables for second AI model and IP
    
    response = call_api(second_ai_model, chat_history, second_ai_ip)  # Call the API
    
    if response.status_code == 200:   # If the response is successful (status code 200)
        
        json_data = response.json()  # Get the JSON data from the response
        
        second_ai_response = json_data['message']  # Get the message from the JSON data

        second_ai_response["role"] = "user" # Set the role to user
            
        chat_history.append(second_ai_response)  # Append the response to the chat history
                
        r_string = f"\nSECOND AI RESPONSE {second_ai_model.upper()} - {second_ai_response['content']}" # Create a string to print and write to file
        
        print_write_file(r_string)    # Print and write to file

        first_ai(chat_history)  # Call the first AI model
    
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
    
    date = datetime.now() # Get the current date and time
    
    directory = "Conversations" # Specify the directory to save the conversations
    # Create the directory if it does not exist
    try:
        os.mkdir(directory) # Create the directory
        print("Directory created") # Print a message to the console
    except OSError:
        print("Directory already exists") # Print a message to the console if the directory already exists
    
    filename = f"conversation-{date}.txt" # Create a filename based on the current date and time
    
    file_path = os.path.join(directory, filename) # Create the full path to the file
        
    first_ai(chat_history) # Call the first AI model to start the conversation

    
