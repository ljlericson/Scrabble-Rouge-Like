NumTimesPointsAdded = 0

return function(context)
    local points = 0

    local numFourWords = 0
    for _, word in ipairs(context.words) do
        if #word == 4 then
            points = points + 50
            NumTimesPointsAdded = NumTimesPointsAdded + 1
            numFourWords = numFourWords + 1
        end
    end
    if numFourWords < NumTimesPointsAdded then
        points = points - (50 * NumTimesPointsAdded - numFourWords)
        NumTimesPointsAdded = NumTimesPointsAdded - 1
    end

    return {
        addScore = points
    }
end