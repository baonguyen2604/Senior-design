# Command scheme between MCU-A and MCU-B
- Commands from A to B and responses:
    - `10`: ping/check status
        - Response:
            - `0`: active
            - other: not active / errors (needs better error classification)
    - `11`: run benchmark (blocking call)
        - Response:
            - `0`: results were okay
            - `1`: error starting benchmark
            - `2`: benchmark ran but there were errors
    
            