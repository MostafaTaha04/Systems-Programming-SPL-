package bgu.spl.net.impl.stomp;

import java.nio.charset.StandardCharsets;
import java.util.Arrays;

import bgu.spl.net.api.MessageEncoderDecoder;

public class StompMessageEncoderDecoder implements MessageEncoderDecoder<String> {
    private byte[] buffer = new byte[1024];
    private int length = 0;

    public String decodeNextByte(byte nextByte){
        if(nextByte == '\u0000'){
            return popFrame();
        }
        pushByte(nextByte);
        return null;
    }

    public byte[] encode(String message){
        return (message + "\u0000").getBytes(StandardCharsets.UTF_8);
    }

    private String popFrame() {
        if (length == 0) {
            return "";
        }
        String frame = new String(buffer, 0, length, StandardCharsets.UTF_8);
        length = 0; 
        return frame;
    }

    private void pushByte(byte nextByte) {
        if (length >= buffer.length) {
            buffer = Arrays.copyOf(buffer, buffer.length * 2); 
        }
        buffer[length++] = nextByte;
    }
}
