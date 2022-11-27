# turbo_sechoir

## Étapes pour démarrer l'interface

1. Démarrer l'environnement virtuel python:
    - Dans un PowerShell Windows : 
    ```
    SSRS_App/venv/Scripts/activate
    ```
    - Si la commande ne fonctionne pas, exécuter:
    ```
    Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope Process
    ```

2. Démarrer l'application flask
    - Dans un PowerShell Windows :
    ```
    cd SSRS_App/
    ```
    ```
    flask run
    ```
