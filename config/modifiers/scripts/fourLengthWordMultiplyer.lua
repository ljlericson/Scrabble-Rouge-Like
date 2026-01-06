return function(context)
    if context.event == "wordScored" then

        local numWords = 0
        for index, word in ipairs(context.words) do
            if #word == 3 then
                numWords = numWords + 1
            end
        end

        local points = 50 * numWords
        if points ~= 0 then
            print(points)
            print(context.words)
        end
        return {
            addScore = points,
            mulMultScore = 1,
            addMultScore = 0

        }
    else
        return {
            addScore = 0,
            mulMultScore = 1,
            addMultScore = 0
        }
    end
end