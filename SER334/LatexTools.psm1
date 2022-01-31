Function Build-LatexToPdf {
<#
  .SYNOPSIS
    
  .DESCRIPTION
    
  .EXAMPLE
      
  .NOTES
    
  #>
  [CmdletBinding()]
  param(
    [string]
    [Alias("f")]
    [Parameter(Mandatory=$true)]
    [ValidateScript({Test-Path $_})]
    $File
  )
  process {
    Try {
      $fileName = [System.IO.Path]::GetFileNameWithoutExtension($File)
      pdflatex $File
      bibtex "$fileName.aux"
      pdflatex $File
      pdflatex $File
    } Catch {
        Write-Error "$($_.Exception.Message) - Line Number: $($_.InvocationInfo.ScriptLineNumber)"
    }
  }
}
