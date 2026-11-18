import subprocess
import re

def obter_senhas_wifi():
    try:
        # Executa o comando para listar todos os perfis de rede salvos
        output_comandos = subprocess.check_output(
            ['netsh', 'wlan', 'show', 'profiles'], 
            stderr=subprocess.STDOUT
        ).decode('latin1')
        
        # Extrai os nomes das redes (perfis)
        perfis = re.findall(r"All User Profile\s*:\s*(.*)", output_comandos)
        
        print(f"{'Rede Wi-Fi':<30} | {'Senha':<}")
        print("-" * 50)
        
        for rede in perfis:
            rede = rede.strip()
            try:
                # Consulta os detalhes de cada rede incluindo a chave clara (senha)
                resultado_rede = subprocess.check_output(
                    ['netsh', 'wlan', 'show', 'profile', name := rede, 'key=clear'], 
                    stderr=subprocess.STDOUT
                ).decode('latin1')
                
                # Procura pelo campo de conteúdo da chave (senha)
                senha_match = re.search(r"Key Content\s*:\s*(.*)", resultado_rede)
                
                if senha_match:
                    senha = senha_match.group(1).strip()
                else:
                    senha = "Aberta / Sem senha"
                    
                print(f"{rede:<30} | {senha:<}")
                
            except subprocess.CalledProcessError:
                print(f"{rede:<30} | {'Erro ao recuperar senha'}")
                
    except Exception as e:
        print(f"Ocorreu um erro ao executar o comando: {e}")

if __name__ == "__main__":
    obter_senhas_wifi()
