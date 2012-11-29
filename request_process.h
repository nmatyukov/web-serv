
int process(FILE *f, int s, char *path)
{
    char buf[4096];
    char *path_local;
    char *method;
    char *protocol;
    struct stat statbuf;

  recv(s, buf, sizeof(buf), 0);             //-------recv()-------------------------
  printf("URL: %s", buf);

  method = strtok(buf, " ");
  path_local = strtok(NULL, " ");
  protocol = strtok(NULL, "\r");
  fseek(f, 0, SEEK_CUR); 
  
  strcat(path, path_local);                //----------/path/path_local

  if (strcmp(method, "GET") != 0)
    send_error(f, 501, "Not supported", NULL, "Method is not supported.");
  else if (stat(path, &statbuf) < 0)
    send_error(f, 404, "Not Found", NULL, "File not found.");

  else
    send_file(f, path, &statbuf);         //-------struct stat *statbuf----file_modification_time------

  return 0;
}

