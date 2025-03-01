#include<stdio.h>
#include<string.h>
#include <poppler/glib/poppler.h>

int main(int argc,char **argv){
    if(argc!=3){
	printf("Usage: %s<pdf-file>\n",argv[0]);
    }
     char *pdf_file = realpath(argv[1],NULL);
     char *pdf_file_2 = realpath(argv[2],NULL);

    if(!pdf_file){
	printf("Error resolving file path\n");
	return 1;
    }
    if(!pdf_file_2){
	printf("Error resolving file path of 2nd pdf\n");
	return 1;
    }
    GError *error = NULL;
    GError *error2 = NULL;
    size_t uri_length = strlen("file://")+strlen(pdf_file)+1;
    char *pdf_uri = malloc(uri_length);
    size_t uri_length_2 = strlen("file://")+strlen(pdf_file_2)+1;
    char *pdf_uri_2 = malloc(uri_length_2);
    
    if(!pdf_uri){
	printf("Memory allocation failed\n");
	free(pdf_file);
	return 1;
    }
    if(!pdf_uri_2){
	printf("Memory allocation failed 2nd pdf\n");
	free(pdf_file_2);
	return 1;
    }
    snprintf(pdf_uri, uri_length, "file://%s",pdf_file);
    snprintf(pdf_uri_2,uri_length_2,"file://%s",pdf_file_2);

    free(pdf_file);
    free(pdf_file_2);



    //load pdf document 
    PopplerDocument *document = poppler_document_new_from_file(pdf_uri,NULL,&error);
    PopplerDocument *document_2 = poppler_document_new_from_file(pdf_uri_2,NULL,&error2);
    if(!document){
	    printf("Error loading 1st PDF: %s\n",error->message);
	    g_error_free(error);
	    return 1;
    }
    if(!document_2){
	    printf("Error loading 2nd PDF: %s\n",error2->message);
	    g_error_free(error2);
	    return 1;

    }

    //get total number of pages
    int num_pages = poppler_document_get_n_pages(document);
    printf("Total pages 1st pdf: %d\n",num_pages);
    int num_pages_2 =poppler_document_get_n_pages(document_2);
    printf("Total pages of 2nd pdf: %d\n",num_pages_2);
    if(num_pages!=num_pages_2){
	printf("Both pdf files have different number of pages\n");
	g_object_unref(document);
	g_object_unref(document_2);
	return 1;
    }
    else{
	if(num_pages==0){
	    printf("Empty PDF!!\n");
	    g_object_unref(document);
	    g_object_unref(document_2);
	    return 1;

	}
	for(int i =0;i<num_pages;i++){
	    PopplerPage *page_pdf_1 = poppler_document_get_page(document,i);
	    PopplerPage *page_pdf_2 = poppler_document_get_page(document_2,i);
	    gchar *text = poppler_page_get_text(page_pdf_1);
	    gchar *text_2 = poppler_page_get_text(page_pdf_2);
	    printf("PDF 1 TEXT:\n");
	    printf("%s\n",text);
	    printf("PDF 2 TEXT:\n");
	    printf("%s\n",text_2);
	    if(strcmp(text,text_2)!=0){
		    printf("PDFs are different.\n");
		    g_object_unref(document);
		    g_object_unref(document_2);
		    g_object_unref(page_pdf_1);
		    g_object_unref(page_pdf_2);
		    g_free(text);
		    g_free(text_2);

		    return 1;
	    }
	      g_object_unref(page_pdf_1);
		    g_object_unref(page_pdf_2);
		    g_free(text);
		    g_free(text_2);


	}
		   	

	printf("Both PDFs are identical.\n");
	
    }
    return 0;
}
