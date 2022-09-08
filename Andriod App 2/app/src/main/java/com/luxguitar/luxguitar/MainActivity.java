package com.luxguitar.luxguitar;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.webkit.WebView;
import android.webkit.WebSettings;
import android.webkit.WebChromeClient;
import android.webkit.WebViewClient;

public class MainActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // Hide top bar
        getSupportActionBar().hide();

        //More code
        setContentView(R.layout.activity_main);
        WebView webView = findViewById(R.id.webView);

        //Enable js?
        webView.setWebChromeClient(new WebChromeClient());
        webView.setWebViewClient(new WebViewClient());
        WebSettings webSettings = webView.getSettings();
        webSettings.setJavaScriptEnabled(true);
//        webSettings.setDomStorageEnabled(true);

        webView.loadUrl("file:///android_asset/index.html");
    }
}


//package com.luxguitar.luxguitar;
//import android.webkit.WebView;
//import android.app.Activity;
//import android.os.Bundle;
//
//public class ViewWeb extends Activity {
//    @Override
//    public void onCreate(Bundle savedInstanceState) {
//        super.onCreate(savedInstanceState);
//        setContentView(R.layout.webview);
//        WebView wv;
//        wv = (WebView) findViewById(R.id.webView1);
//        wv.loadUrl("file:///android_asset/aboutcertified.html");   // now it will not fail here
//    }
//}